#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <memory>
#include <string>
#include "Direct3D.h"
#include "Light.h"
#include "RayCastData.h"
#include "Transform.h"

#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

//�N���X�̑O���錾
class Texture;

class Fbx
{
	//�}�e���A��
	struct MATERIAL
	{
		std::unique_ptr<Texture> pTexture;
		XMFLOAT4 diffuse = {};
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;
		XMFLOAT4	chroma;
		XMFLOAT4	light;
		int			isTexture;
		float		bright;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	std::unique_ptr<ID3D11Buffer*[]> pIndexBuffer_;	//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	std::unique_ptr<MATERIAL[]> pMaterialList_;
	std::unique_ptr<int[]> indexCount_;
	int vertexCount_;	//���_��
	int polygonCount_;	//�|���S����
	int materialCount_;	//�}�e���A���̌�
	std::unique_ptr<VERTEX[]> pVertices_;	//���_�������\���̂̃|�C���^
	std::unique_ptr<std::unique_ptr<int[]>[]> ppIndex_;
public:
	Fbx();
	~Fbx();
	HRESULT Load(const std::string& fileName);
	void InitVertex(fbxsdk::FbxMesh* pMesh);	//���_�o�b�t�@����
	void InitIndex(fbxsdk::FbxMesh* pMesh);	//�C���f�b�N�X�o�b�t�@����
	HRESULT IntConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void RayCast(RayCastData& rayData);
	void Draw(Transform& transform, float Alpha, const Light *lightpos, SHADER_TYPE TYPE);
	void Draw(Transform& transform, XMFLOAT3 Chroma, float Bright, float Alpha, const Light* lightpos, SHADER_TYPE TYPE);	//transform, RGB�̒l, ���x(0 �`255)
	void Release();

	void SetTexture(const Texture* tex);

	XMVECTOR NormalDotLight(Transform& tr);
};