#pragma once
#include "GameObject.h"
#include "BillBoard.h"
#include <list>


//�G�~�b�^�[�i�p�[�e�B�N���̔������j����鎞�̃f�[�^�ꗗ
//Start�֐��̈����Ƃ��Ďg��
struct EmitterData
{
	std::string textureFileName;	//�摜�t�@�C����
	XMFLOAT3 position;		//�ʒu
	XMFLOAT3 positionErr;	//�ʒu�̌덷
	XMFLOAT3 dir;			//�p�[�e�B�N���̈ړ�����
	XMFLOAT3 dirErr;		//�ړ������̌덷�i�e���̊p�x�j
	float	 speed;			//1�t���[���̑��x
	float	 speedErr;		//���x�덷�i0�`1�j
	float	 accel;			//�����x
	float	 gravity;		//�d��
	XMFLOAT4 color;			//�F�iRGBA 0�`1�j
	XMFLOAT4 deltaColor;	//�F�̕ω���
	XMFLOAT2 size;			//�T�C�Y
	XMFLOAT2 sizeErr;		//�T�C�Y�덷�i0�`1�j
	XMFLOAT2 scale;			//1�t���[���̊g�嗦
	float    lifeTime;		//�p�[�e�B�N���̎����i�t���[�����j

	int delay;			//���t���[�������Ƀp�[�e�B�N���𔭐������邩
	int number;			//1�x�ɏo���p�[�e�B�N����

	//������
	EmitterData()
	{
		textureFileName = "";
		position = positionErr = dir = dirErr = XMFLOAT3(0, 0, 0);
		speed = 0.0f;
		accel = 1.0f;
		gravity = 0.0f;
		color = XMFLOAT4(1, 1, 1, 1);
		deltaColor = XMFLOAT4(0, 0, 0, 0);
		size = scale = XMFLOAT2(1.0f, 1.0f);
		lifeTime = 30.0f;
	}
};



//�G�t�F�N�g���Ǘ�����N���X
class Particle : public GameObject
{
	//�G�~�b�^�[�i�p�[�e�B�N���̕��ˌ��j
	struct Emitter
	{
		EmitterData data ;		//�쐬���Ɏw�肳�ꂽ�f�[�^
		int handle = -1;		//�n���h���i�ԍ��j
		DWORD frameCount = 0;	//�J�n���Ă���̃t���[����
		BillBoard* pBillBoard = nullptr;	//�p�[�e�B�N���Ɏg���|���S��
		bool isDead = false;	//�폜�Ώۂ��ǂ����i���ۂ̓p�[�e�B�N�����S��������܂ŃG�~�b�^�[�͏����Ȃ��j
		int particleNum = 0;	//���̃G�~�b�^�[����o�������p�[�e�B�N���̐�
	};


	//�p�[�e�B�N���̕ω�����f�[�^
	struct Data
	{
		XMFLOAT3 position;	//�ʒu
		XMFLOAT2 scale;		//�T�C�Y
		XMFLOAT4 color;		//�F
	};

	//�p�[�e�B�N��1���̃f�[�^
	struct ParticleData
	{
		Data now;			//���݂̏��
		Data delta;			//1�t���[���̕ω���
		DWORD life;			//�c�����
		float accel;		//�����x
		float gravity;		//�d��
		Emitter* pEmitter;	//������
	};

	
	std::list<Emitter*>			emitterList_;	//�G�~�b�^�[�B
	std::list<ParticleData*>	particleList_;	//�p�[�e�B�N���B


	//�������̃p�[�e�B�N�����X�V
	void ParticleUpdate();

	//�G�~�b�^�̍X�V�i�^�C�~���O����Ńp�[�e�B�N���𔭐�������j
	void EmitterUpdate();

public:
	//�R���X�g���N�^
	Particle(GameObject* parent);

	//�f�X�g���N�^
	~Particle();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;


	//�G�~�b�^���쐬�i�G�t�F�N�g�J�n�j
	//�����FemitterData	�e����
	//�ߒl�F�G�t�F�N�g�i�G�~�b�^�j�̔ԍ�
	int Start(EmitterData emitterData);

	//�G�~�b�^���폜�i�G�t�F�N�g�I���j
	//�����Fhandle	�G�t�F�N�g�̔ԍ�
	void End(int handle);
};

