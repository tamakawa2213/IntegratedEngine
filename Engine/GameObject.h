//�S�ẴQ�[���I�u�W�F�N�g�̐e�ƂȂ�N���X
#pragma once

#include <list>
#include <memory>
#include <string>
#include "AssignmentFunction.h"
#include "CallDef.h"
#include "Transform.h"
#include "SphereCollider.h"

class GameObject
{
	GameObject* pParent_;				//�e�N���X�̃|�C���^
	const std::string	objectName_;	//�I�u�W�F�N�g�̖��O
	bool KILL;							//true�Ȃ�I�u�W�F�N�g����

protected:
	Transform	transform_;						//Transform
	std::list<std::shared_ptr<GameObject>> childList_;			//�Q�[���I�u�W�F�N�g�̃��X�g�\��
	std::list<SphereCollider*> Colliderlist_;	//Collider�̃��X�g�\��
	SphereCollider* Collision_;					//Collider�̃|�C���^

public:
	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	virtual ~GameObject();
	AssignmentFunction assFunc_;				//�֐��̎w��񐔎��s�N���X

	virtual void Initialize() {}	//������
	virtual void Update() {}		//�X�V
	void UpdateSub();				//�q�I�u�W�F�N�g�̍X�V
	virtual void FixedUpdate() {}
	void FixedUpdateSub();
	virtual void Draw() {}			//�`��
	void DrawSub();					//�q�I�u�W�F�N�g�̕`��
	virtual void DrawUnique(){}
	void DrawUniqueSub();
	virtual void Release() {}		//���
	void ReleaseSub();

	///////////////////////////////////////////transform�Ɋւ���Z�b�^�A�Q�b�^//////////////////////////////////////////////////
	void SetPosition(const Position& position) { transform_.position_ = position; }
	void SetRotate(const XMFLOAT3& rotate) { transform_.rotate_ = rotate; }
	void SetScale(const Scale& scale) { transform_.scale_ = scale; }
	const Position GetPosition() { return transform_.position_; }
	const XMFLOAT3 GetRotate() { return transform_.rotate_; }
	const Scale GetScale() { return transform_.scale_; }
	const Transform GetTransform() { return transform_; }

	///////////////////////////////////////////�I�u�W�F�N�g�����ł�����Z�b�g///////////////////////////////////////////////////
	void KillMe() { KILL = true; }							//���ꂪ�Ă΂ꂽ��Y���I�u�W�F�N�g�͏���
	void KillAllChildren();									//�q�I�u�W�F�N�g������
	void KillAllChildren(GameObject* object);				//�����Ŏ󂯎�����I�u�W�F�N�g�̎q�I�u�W�F�N�g�����ł�����

	/////////////////////////////////////////////��������擾�������///////////////////////////////////////////////////////////
	GameObject* FindChildObject(const std::string& ObjectName);		//�����Ŏ󂯎�������O�Ɠ������O�̃I�u�W�F�N�g�����g�̎q���ɂ��Ȃ�����������֐�
	GameObject* GetRootJob();									//RootJob��T���֐�
	GameObject* FindObject(const std::string& ObjectName);				//�����Ŏ󂯎�������O�Ɠ������O�̃I�u�W�F�N�g��T���֐�
	const std::string GetObjectName() { return objectName_; }	//�I�u�W�F�N�g�̖��O���擾
	GameObject* GetParent() { return pParent_; }				//�e�A�h���X���擾
	bool HasChild();

	///////////////////////////////////////////////////Collision�֌W////////////////////////////////////////////////////////////
	void AddCollider(SphereCollider* Collider);				//Collider��GameObject�����т��Alist�ɒǉ�����
	void Collision(GameObject* pTarget);					//�����蔻����s��
	virtual void OnCollision(GameObject* pTarget) {}		//���������炱�ꂪ�Ă΂��(override��p)

	//////////////////////////////////////////////////Debug���[�h��p///////////////////////////////////////////////////////////
	virtual void ShowGraphical() {}							//ImGui�\��
	virtual void DebugMode() {}								//�f�o�b�O���[�h�ő��삷�邱��

	//�N���X�e���v���[�g
	template <class T>
	T* Instantiate(GameObject* pParent)
	{
		std::shared_ptr<T> pNewObject = std::make_shared<T>(pParent);
		if (pParent != nullptr)
		{
			pParent->childList_.push_back(pNewObject);
		}
		pNewObject->Initialize();
		return pNewObject.get();
	}
};