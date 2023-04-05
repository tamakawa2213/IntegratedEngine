#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

using namespace DirectX;

class Texture
{
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
	UINT imgWidth_;
	UINT imgHeight_;
	XMMATRIX size_;
public:
	Texture();
	~Texture();
	HRESULT Load(const LPCWSTR& fileName);
	HRESULT Load(const std::string& fileName);
	void Release();

	ID3D11SamplerState* GetSampler() { return pSampler_; }
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; }
	UINT GetImgWidth() { return imgWidth_; }
	UINT GetImgHeight() { return imgHeight_; }
	XMMATRIX GetSize() { return size_; }
};