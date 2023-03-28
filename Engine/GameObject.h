//全てのゲームオブジェクトの親となるクラス

#pragma once

#include <list>
#include <string>
#include "AssignmentFunction.h"
#include "CallDef.h"
#include "Transform.h"
#include "SphereCollider.h"

class GameObject
{
	GameObject* pParent_;				//親クラスのポインタ
	const std::string	objectName_;	//オブジェクトの名前
	bool KILL;							//trueならオブジェクト消滅

protected:
	AssignmentFunction assFunc_;				//関数の指定回数実行クラス
	Transform	transform_;						//Transform
	std::list<GameObject*> childList_;			//ゲームオブジェクトのリスト構造
	std::list<SphereCollider*> Colliderlist_;	//Colliderのリスト構造
	SphereCollider* Collision_;					//Colliderのポインタ

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	virtual ~GameObject();

	virtual void Initialize() {}	//初期化
	virtual void Update() {}		//更新
	void UpdateSub();				//子オブジェクトの更新
	virtual void FixedUpdate() {}
	void FixedUpdateSub();
	virtual void Draw() {}			//描画
	void DrawSub();					//子オブジェクトの描画
	virtual void DrawUnique(){}
	void DrawUniqueSub();
	virtual void Release() {}		//解放
	void ReleaseSub();

	///////////////////////////////////////////transformに関するセッタ、ゲッタ//////////////////////////////////////////////////
	void SetPosition(XMFLOAT3 position) { transform_.position_ = position; }
	void SetRotate(XMFLOAT3 rotate) { transform_.rotate_ = rotate; }
	void SetScale(XMFLOAT3 scale) { transform_.scale_ = scale; }
	const XMFLOAT3 GetPosition() { return transform_.position_; }
	const XMFLOAT3 GetRotate() { return transform_.rotate_; }
	const XMFLOAT3 GetScale() { return transform_.scale_; }
	const Transform GetTransform() { return transform_; }

	///////////////////////////////////////////オブジェクトを消滅させるセット///////////////////////////////////////////////////
	void KillMe() { KILL = true; }							//これが呼ばれたら該当オブジェクトは消滅
	void KillAllChildren();									//子オブジェクトを消滅
	void KillAllChildren(GameObject* object);				//引数で受け取ったオブジェクトの子オブジェクトを消滅させる

	/////////////////////////////////////////////何かしら取得するもの///////////////////////////////////////////////////////////
	GameObject* FindChildObject(std::string ObjectName);		//引数で受け取った名前と同じ名前のオブジェクトが自身の子供にいないか検索する関数
	GameObject* GetRootJob();									//RootJobを探す関数
	GameObject* FindObject(std::string ObjectName);				//引数で受け取った名前と同じ名前のオブジェクトを探す関数
	const std::string GetObjectName() { return objectName_; }	//オブジェクトの名前を取得
	GameObject* GetParent() { return pParent_; }				//親アドレスを取得
	bool HasChild();

	///////////////////////////////////////////////////Collision関係////////////////////////////////////////////////////////////
	void AddCollider(SphereCollider* Collider);				//ColliderとGameObjectを結びつけ、listに追加する
	void Collision(GameObject* pTarget);					//当たり判定を行う
	virtual void OnCollision(GameObject* pTarget) {}		//当たったらこれが呼ばれる(override専用)

	//////////////////////////////////////////////////Debugモード専用///////////////////////////////////////////////////////////
	virtual void ShowGraphical() {}							//ImGui表示
	virtual void DebugMode() {}								//デバッグモードで操作すること

	//クラステンプレート
	template <class T>
	T* Instantiate(GameObject* pParent)
	{
		T* pNewObject = new T(pParent);
		if (pParent != nullptr)
		{
			pParent->childList_.push_back(pNewObject);
		}
		pNewObject->Initialize();
		return pNewObject;
	}
};