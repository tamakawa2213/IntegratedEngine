#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize(int winW, int winH);

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(const XMVECTOR& position);
	void SetPosition(const XMFLOAT3& position);

	//焦点（見る位置）を設定
	void SetTarget(const XMVECTOR& target);
	void SetTarget(const XMFLOAT3& target);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	XMFLOAT3 GetCameraPosition();
	XMFLOAT3 GetCameraTarget();
	XMVECTOR GetCameraVecPosition();
	XMVECTOR GetCameraVecTarget();

	//カメラの視線ベクトルを取得
	XMVECTOR GetSight();

	//プロジェクション行列をセット
	void SetProjMatrix(int winW, int winH, int split);

	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix();
};