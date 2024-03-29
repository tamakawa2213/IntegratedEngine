#include "Camera.h"
#include "CallDef.h"
#include "Sprite.h"

using namespace DirectX;

Sprite::Sprite() :pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr), index_(0)
{
}

//デストラクタでRELEASEを呼び出す
Sprite::~Sprite()
{
	Release();
}


HRESULT Sprite::Initialize(const LPCWSTR& filename)
{
	HRESULT hr;

	// 頂点情報
	VERTEX vertices[] =
	{
		{ XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（左上）
		{ XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（右上）
		{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（右下）
		{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（左下）
	};

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	HR_FAILED(hr, L"バッファの作成に失敗しました");

	//インデックス情報
	int index[] = { 0,1,2, 0,2,3 };
	index_ = (int)std::size(index);

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.ByteWidth = sizeof(index);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	HR_FAILED(hr, L"インデックスバッファの作成に失敗しました");

	hr = ConBuf(filename);
	return hr;
}

HRESULT Sprite::ConBuf(const LPCWSTR& filename) {
	HRESULT hr;
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	HR_FAILED(hr, L"コンスタントバッファの作成に失敗しました");

	pTexture_ = std::make_unique<Texture>();
	hr = Load(filename);

	return hr;
}

HRESULT Sprite::Load(const LPCWSTR& filename) {
	HRESULT hr;
	//テクスチャをロード
	hr = pTexture_->Load(filename);
	return hr;
}

void Sprite::Draw(Transform& transform, float alpha)
{
	Direct3D::SetShader(SHADER_TYPE::Dimension2);
	CONSTANT_BUFFER cb;

	//行列の計算をして、ワールド行列を返す
	transform.Calclation();
	XMMATRIX mat = XMMatrixScaling(1.0f / Direct3D::scrWidth, 1.0f / Direct3D::scrHeight, 1.0f);
	cb.matW = XMMatrixTranspose(pTexture_->GetSize() * mat * transform.GetWorldMatrix());
	cb.alpha = alpha;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	//インデックスバッファをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
	Direct3D::pContext->DrawIndexed(index_, 0, 0);

	Direct3D::SetShader(SHADER_TYPE::Dimension3);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

UINT Sprite::GetImgWidth()
{
	return pTexture_->GetImgWidth();
}

UINT Sprite::GetImgHeight()
{
	return pTexture_->GetImgHeight();
}
