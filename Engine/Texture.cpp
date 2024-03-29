#include <wincodec.h>
#include "Texture.h"
#include "CallDef.h"
#include "Direct3D.h"
#include <string>

#pragma comment( lib, "WindowsCodecs.lib" )

Texture::Texture() : pSampler_(nullptr), pSRV_(nullptr), imgWidth_(0), imgHeight_(0), size_()
{
}

Texture::~Texture()
{
	Release();
}

HRESULT Texture::Load(const LPCWSTR& fileName)
{
	HRESULT hr;

	//画像ファイルのロード
	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	if(FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	hr = pFactory->CreateDecoderFromFilename(fileName, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	hr = pDecoder->GetFrame(0, &pFrame);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	hr = pFactory->CreateFormatConverter(&pFormatConverter);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	hr = pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}

	//画像サイズを取得
	hr = pFormatConverter->GetSize(&imgWidth_, &imgHeight_);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	size_ = XMMatrixScaling((float)imgWidth_, (float)imgHeight_, 0);

	//テクスチャを作成
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth_;
	texdec.Height = imgHeight_;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	hr = Direct3D::pDevice->CreateTexture2D(&texdec, nullptr, &pTexture);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}

	//テクスチャをデバイスコンテキストに渡す
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	hr = Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	hr = pFormatConverter->CopyPixels(nullptr, imgWidth_ * 4, imgWidth_ * imgHeight_ * 4, (BYTE*)hMappedres.pData);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}
	Direct3D::pContext->Unmap(pTexture, 0);

	//サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	//WRAP, MIRROR, CLAMPでテクスチャの貼り方が変わる
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}

	//シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	if (FAILED(hr)) [[unlikely]] {
		SAFE_RELEASE(pTexture);
		SAFE_RELEASE(pFormatConverter);
		SAFE_RELEASE(pFrame);
		SAFE_RELEASE(pDecoder);
		SAFE_RELEASE(pFactory);
		return hr;
	}

	//解放処理
	SAFE_RELEASE(pTexture);
	SAFE_RELEASE(pFormatConverter);
	SAFE_RELEASE(pFrame);
	SAFE_RELEASE(pDecoder);
	SAFE_RELEASE(pFactory);

	return hr;
}

HRESULT Texture::Load(const std::string& fileName)
{
	wchar_t file[CHAR_MAX];
	size_t ret;
	mbstowcs_s(&ret, file, fileName.c_str(), fileName.length());
	return Load(file);
}

void Texture::Release()
{
	SAFE_RELEASE(pSampler_);
	SAFE_RELEASE(pSRV_);
}