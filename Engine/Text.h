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

    //���_���
    struct VERTEX
    {
        XMVECTOR position;
        XMVECTOR uv;
    };

	float fontSize = 50.0f;			    	//�t�H���g�T�C�Y
	std::string stringList;                 //�����񃊃X�g


    ID3D11Texture2D* m_layerBuffer;
    ID3D11ShaderResourceView* m_shaderResourceView;
    ID3D11SamplerState* pSampler_;
    ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
    ID3D11Buffer* pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
    ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@

    HRESULT InitChar(LPCWSTR c);
    HRESULT InitChar(std::string c);
public:
    //�R���X�g���N�^
    Text();

    //�f�X�g���N�^
    ~Text();

    //�ꕶ���̐���

    void Initialize(std::string text);

    void Draw();
};