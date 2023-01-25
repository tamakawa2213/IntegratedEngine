#include "Camera.h"
#include "CallDef.h"

//変数
namespace Camera {
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
	XMMATRIX billboard_;

//初期化
	void Initialize(int winW, int winH)
	{
		position_ = XMVectorSet(0, 5, 1.4f, 0);	//カメラの位置
		target_ = XMVectorSet(0, 0, 1.5f, 0);		//カメラの焦点
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)winW / (FLOAT)winH / 2.0f, 0.1f, 1000.0f);
	}

	//更新
	void Update()
	{
		//ビュー行列の作成(カメラ固定のゲームならInitializeに書く)
		viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

		billboard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, XMVectorSet(0, 1, 0, 0));
		billboard_ = XMMatrixInverse(nullptr, billboard_);
	}

	//位置を設定
	void SetPosition(XMVECTOR position)
	{
		position_ = position;
	}

	void SetPosition(XMFLOAT3 position)
	{
		position_ = XMLoadFloat3(&position);
	}

	//焦点を設定
	void SetTarget(XMVECTOR target)
	{
		target_ = target;
	}

	void SetTarget(XMFLOAT3 target)
	{
		target_ = XMLoadFloat3(&target);
	}

	//ビュー行列を取得
	XMMATRIX GetViewMatrix()
	{
		return viewMatrix_;
	}

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix()
	{
		return projMatrix_;
	}
	XMFLOAT3 GetCameraPosition()
	{
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, position_);
		return pos;
	}
	XMFLOAT3 GetCameraTarget()
	{
		XMFLOAT3 tar;
		XMStoreFloat3(&tar, target_);
		return tar;
	}
	XMVECTOR GetCameraVecPosition()
	{
		return position_;
	}
	XMVECTOR GetCameraVecTarget()
	{
		return target_;
	}

	XMVECTOR GetSight()
	{
		return target_ - position_;
	}

	void SetProjMatrix(int winW, int winH, int split)
	{
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)winW / (FLOAT)winH / split, 0.1f, 1000.0f);
	}


	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix()
	{
		return billboard_;
	}
}