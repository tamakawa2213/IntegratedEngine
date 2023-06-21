#include <math.h>
#include "Debug.h"
#include "GameObject.h"

GameObject::GameObject() : GameObject(nullptr, "")
{
}

GameObject::GameObject(GameObject* parent, const std::string& name)
	: childList_(), transform_(), pParent_(parent), objectName_(name), KILL(false), assFunc_()
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

	for (auto&& itr : childList_) {
		itr->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();) {
		if ((*itr)->KILL) {
			(*itr)->ReleaseSub();
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

	//�f�o�b�O���[�h���Ăяo���Ă��Ȃ��Ƃ����s
	if (!Debug::CallDebug_)
#endif
	{
		assFunc_.Update();

		for (auto&& itr : childList_) {
			itr->FixedUpdateSub();
		}

		for (auto itr = childList_.begin(); itr != childList_.end();) {
			if ((*itr)->KILL) {
				(*itr)->ReleaseSub();
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

	for (auto&& itr : childList_) {
		itr->DrawSub();
	}
}

void GameObject::DrawUniqueSub()
{
	DrawUnique();

	for (auto&& itr : childList_) {
		itr->DrawUniqueSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();

	for (auto&& itr : childList_) {
		itr->ReleaseSub();
	}
}

void GameObject::KillAllChildren()
{
	for (auto&& itr : childList_)
	{
		KillAllChildren(itr.get());
	}
	childList_.clear();
}

void GameObject::KillAllChildren(GameObject* object)
{
	for (auto&& itr : object->childList_)
	{
		KillAllChildren(itr.get());
	}
	object->childList_.clear();
	SAFE_RELEASE(object);
}

GameObject* GameObject::FindChildObject(const std::string& ObjectName)
{
	for (auto&& itr : childList_)
	{
		if (itr->objectName_ == ObjectName)
		{
			//this��Ԃ��ƊY���I�u�W�F�N�g�̐e�̃A�h���X���Ԃ��Ă��Ă��܂�
			return itr.get();
		}
		else
		{
			GameObject* obj = itr->FindChildObject(ObjectName);
			if (obj)	//nullptr�łȂ���Δ������Ă���̂�obj��Ԃ�
			{
				return obj;
			}
		}
	}
	return nullptr;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_)
	{
		return pParent_->GetRootJob();
	}
	else
	{
		//pParent��nullptr�Ȃ炻�ꂪRootJob�ł���
		return this;
	}
}

GameObject* GameObject::FindObject(const std::string& ObjectName)
{
	//RootJob(=�S�Ă̐e)�ɖ߂�A�������猟������
	return GetRootJob()->FindChildObject(ObjectName);
}

bool GameObject::HasChild()
{
	return !childList_.empty();
}

void GameObject::AddCollider(std::shared_ptr<SphereCollider> Collider)
{
	Collider->SetGameObject(this);		//GameObject�ɕR�t��
	Colliderlist_.push_back(Collider);	//List�ɓ����
}

void GameObject::Collision(GameObject* pTarget)
{
	//�����Ƃ̓����蔻��͂��Ȃ�
	if (this != pTarget && !this->Colliderlist_.empty() && !pTarget->Colliderlist_.empty())
	{
		bool Called = false;	//OnCollision���Ăяo����Ă�����true�ɂ��A���[�v�𔲂��o��

		//���g�ƑΏۂ̃��X�g�̑�������Ŕ���
		for (auto&& myitr : this->Colliderlist_)
		{
			for (auto&& taritr : pTarget->Colliderlist_)
			{
				if (myitr->Ishit(taritr.get()))
				{
					OnCollision(pTarget);
					Called = true;
					break;
				}
			}
			if (Called)
				break;
		}
	}

	//�q�I�u�W�F�N�g�̔�����Ă�
	if (!pTarget->childList_.empty())
	{
		for (auto&& itr : pTarget->childList_)
		{	//list�̐�������A����
			Collision(itr.get());
		}
	}

}
