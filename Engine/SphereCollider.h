#pragma once

class GameObject;
struct Position;

class SphereCollider
{
	float Radius_;				//���a
	GameObject* pGameObject_;	//GameObject�^�̃|�C���^
public:
	SphereCollider(const Position& position, float Radius);
	~SphereCollider();

	bool Ishit(SphereCollider* pTarget);	//�Ăяo�����ƈ����Ɏ󂯎��������̓����蔻������s����

	float GetRadius() { return Radius_; }

	void SetGameObject(GameObject* obj) { pGameObject_ = obj; }
};