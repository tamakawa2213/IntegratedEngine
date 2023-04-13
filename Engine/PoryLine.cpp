#include "PoryLine.h"
#include "CallDef.h"
#include "Camera.h"
#include <vector>


PoryLine::PoryLine():
	WIDTH_(0.1f),	//太さ
	LENGTH_(30),	//長さ（あくまで位置を記憶する数で、実際の長さは移動速度によって変わる）
	pVertexBuffer_(nullptr), pConstantBuffer_(nullptr), pTexture_(nullptr)
{
}

void PoryLine::AddPosition(XMFLOAT3 pos)
{
	//リストの先頭に現在位置を追加
	positions_.push_front(pos);

	//指定の長さを超えてたら終端のデータを削除
	if (positions_.size() > LENGTH_)
	{
		positions_.pop_back();
	}

	//頂点バッファをクリア（今から作るから）
	SAFE_RELEASE(pVertexBuffer_);


	//現在のカメラの位置をベクトルとして取得
	XMVECTOR vCamPos = Camera::GetCameraVecPosition();

	//頂点データを作るための配列を準備
	int leng = LENGTH_ * 2;
	VERTEX* vertices = new VERTEX[leng];

	//頂点データを作る
	int index = 0;
	auto itr = positions_.begin();
	for (int i = 0; i < LENGTH_; i++ )
	{
		//記憶してた位置
		XMVECTOR vPos = XMLoadFloat3(&(*itr));
		
		itr++;
		if (itr == positions_.end())	break;

		//記憶してた位置から、その次に記憶してた位置に向かうベクトル
		XMVECTOR vLine = XMLoadFloat3(&(*itr)) - vPos;

		//視線とラインに垂直なベクトル
		XMVECTOR vArm = XMVector3Cross(vLine, vCamPos);
		vArm = XMVector3Normalize(vArm) * WIDTH_;


		//頂点情報を入れていく
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, vPos + vArm);
		VERTEX vertex1 = { pos, XMFLOAT3((float)i / LENGTH_, 0, 0) };

		XMStoreFloat3(&pos, vPos - vArm);
		VERTEX vertex2 = { pos, XMFLOAT3((float)i / LENGTH_, 1, 0) };

		int s = sizeof(VERTEX);

		vertices[index] = vertex1;
		index++;
		vertices[index] = vertex2;
		index++;
	}

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * LENGTH_ * 2;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);


	delete[] vertices;
}

HRESULT PoryLine::Load(std::string fileName)
{
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);


	//テクスチャ
	pTexture_ = new Texture;
	pTexture_->Load(fileName.c_str());

	return S_OK;
}

void PoryLine::Draw()
{
	Direct3D::SetShader(SHADER_TYPE::BILLBOARD);
	Direct3D::SetBlendMode(BLEND_DEFAULT);


	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.color = XMFLOAT4(1, 1, 1, 1);

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開



	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);



	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用


	//頂点データの並び方を指定
	Direct3D::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	
	//描画（インデックスバッファいらないタイプ）
	Direct3D::pContext->Draw((UINT)((positions_.size()-1)*2), (UINT)0);

	//頂点データの並び方を指定を戻す
	Direct3D::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void PoryLine::Release()
{
	//SAFE_DELETE(pTexture_);
	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
	positions_.clear();
}
