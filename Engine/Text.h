#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <list>

#pragma comment(lib, "d3d11.lib")

using namespace DirectX;

class Text
{
    struct CONSTANT_BUFFER
    {
        XMMATRIX matW;
    };

    //頂点情報
    struct VERTEX
    {
        XMVECTOR position;
        XMVECTOR uv;
    };

	float fontSize = 50.0f;			    	//フォントサイズ
	std::string stringList;                 //文字列リスト


    ID3D11Texture2D* m_layerBuffer;
    ID3D11ShaderResourceView* m_shaderResourceView;
    ID3D11SamplerState* pSampler_;
    ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
    ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
    ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

    HRESULT InitChar(LPCWSTR c);
    HRESULT InitChar(std::string c);
public:
    //コンストラクタ
    Text();

    //デストラクタ
    ~Text();

    //一文字の生成

    void Initialize(std::string text);

    void Draw();
};