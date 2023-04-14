#include "SphereCollider.h"
#include "GameObject.h"
#include "Math.h"
#include "Position.h"

SphereCollider::SphereCollider(const Position& position, float Radius) : Radius_(Radius), pGameObject_(nullptr)
{
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::Ishit(SphereCollider* pTarget)
{
	if (this != pTarget)
	{
		if (Math::GetDistanceSphere(this->pGameObject_->GetPosition(), pTarget->pGameObject_->GetPosition(), this->Radius_, pTarget->Radius_) == 0)
		{
			return true;
		}
	}
	return false;
}
