#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"
#include <memory>

class Sprite
{
protected:
	//コンスタントバッファ
	struct CONSTANT_BUFFER
	{
		XMMATRIX matW;
		float alpha;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	std::unique_ptr<Texture> pTexture_;
	int index_;		//頂点数

public:
	Sprite();				//コンストラクタ
	~Sprite();				//デストラクタ
	virtual HRESULT Initialize(const LPCWSTR& filename);	//初期化
	HRESULT ConBuf(const LPCWSTR& filename);
	virtual HRESULT Load(const LPCWSTR& filename);
	//Transformを引数にとる
	void Draw(Transform& transform, float alpha);	//描画
	void Release();									//解放
	UINT GetImgWidth();
	UINT GetImgHeight();
	const Texture* GetpTexture() { return pTexture_.get(); }
};