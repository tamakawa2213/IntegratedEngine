#include <math.h>
#include "Debug.h"
#include "GameObject.h"

GameObject::GameObject() : GameObject(nullptr, "")
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	: childList_(), transform_(), pParent_(parent), objectName_(name), KILL(false), Collision_(nullptr), assFunc_()
{
	if (parent)
	{
		transform_.pParent_ = &parent->transform_;
	}
}

GameObject::~GameObject()
{
	Colliderlist_.clear();
}

void GameObject::UpdateSub()
{
	Update();

	for (auto itr : childList_) {
		itr->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->KILL) {
			(*itr)->ReleaseSub();
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else {
			(*itr)->Collision(GetRootJob());
			itr++;
		}
	}
}

void GameObject::FixedUpdateSub()
{
	FixedUpdate();

#if _DEBUG
	DebugMode();
	ShowGraphical();

	//デバッグモードを呼び出していないとき実行
	if (!Debug::CallDebug_)
#endif
	{
		assFunc_.Update();

		for (auto itr : childList_) {
			itr->FixedUpdateSub();
		}

		for (auto itr = childList_.begin(); itr != childList_.end();) {
			if ((*itr)->KILL) {
				(*itr)->ReleaseSub();
				SAFE_DELETE(*itr);
				itr = childList_.erase(itr);
			}
			else {
				(*itr)->Collision(GetRootJob());
				itr++;
			}
		}
	}
}

void GameObject::DrawSub()
{
	Draw();

	for (auto itr : childList_) {
		itr->DrawSub();
	}
}

void GameObject::DrawUniqueSub()
{
	DrawUnique();

	for (auto itr : childList_) {
		itr->DrawUniqueSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();

	for (auto itr : childList_) {
		itr->ReleaseSub();
	}
}

void GameObject::KillAllChildren()
{
	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		KillAllChildren(*itr);
		SAFE_DELETE(*itr);
		itr = childList_.erase(itr);
	}
	childList_.clear();
}

void GameObject::KillAllChildren(GameObject* object)
{
	for (auto itr = object->childList_.begin(); itr != object->childList_.end();)
	{
		KillAllChildren(*itr);
		SAFE_DELETE(*itr);
		itr = object->childList_.erase(itr);
	}
	object->childList_.clear();
	SAFE_RELEASE(object);
}

GameObject* GameObject::FindChildObject(std::string ObjectName)
{
	for (auto itr : childList_)
	{
		if (itr->objectName_ == ObjectName)
		{
			//thisを返すと該当オブジェクトの親のアドレスが返ってきてしまう
			return itr;
		}
		else
		{
			GameObject* obj = itr->FindChildObject(ObjectName);
			if (obj != nullptr)
			{
				return obj;
			}
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ != nullptr)
	{
		return pParent_->GetRootJob();
	}
	else
	{
		//pParentがnullptrならそれがRootJobである
		return this;
	}
}

GameObject* GameObject::FindObject(std::string ObjectName)
{
	//RootJob(=全ての親)に戻り、そこから検索する
	return GetRootJob()->FindChildObject(ObjectName);
}

bool GameObject::HasChild()
{
	return !childList_.empty();
}

void GameObject::AddCollider(SphereCollider* Collider)
{
	Collider->SetGameObject(this);		//GameObjectに紐付け
	Colliderlist_.push_back(Collider);	//Listに入れる
}

void GameObject::Collision(GameObject* pTarget)
{
	//自分との当たり判定はしない
	if (this != pTarget && pTarget->Collision_ != nullptr)
	{
		//リストのサイズだけループ
		for (auto itr = Colliderlist_.begin(); itr != Colliderlist_.end(); itr++)	//外すとなぜかエラー
		{
			if (Collision_->Ishit(pTarget->Collision_))
			{
				OnCollision(pTarget);
			}
		}
	}
		//子オブジェクトの判定も呼ぶ
		if (!pTarget->childList_.empty())
		{
			for (auto itr : pTarget->childList_)
			{	//listの数だけ回帰処理
				Collision(itr);
			}
		}
	
}
