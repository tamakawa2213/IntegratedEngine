#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class Transform;

namespace Math
{
	float Det(const XMFLOAT3& a, const XMFLOAT3& b, const XMFLOAT3& c);

	//�����ƃ|���S���̓����蔻����s���֐�
	//������ : �����̎n�_�@������ : �����̏I�_�@��O�`��܈��� : �|���S���̊e���_
	bool SegmentToPlane(const XMFLOAT3& segstart, const XMFLOAT3& segend, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2);

	//�������Ԃ������蔻��
	bool Intersect(const XMFLOAT3& start, const XMFLOAT3& dir, const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2, float* dist = nullptr);

	/// <summary>�E�F�[�u���b�g�֐��𐶐����Ďw�肵���ʒu�̒l��Ԃ��֐�</summary>
	/// <param name="Incli">�X��</param>
	/// <param name="Coord">�擾�������l�̍��W</param>
	/// <returns>�w�肵���l�̍��W</returns>
	float Wavelet(float Incli, float Coord);

	/// <summary>�E�F�[�u���b�g�֐��𐶐����Ďw�肵��2�������W��Ԃ��֐�</summary>
	/// <param name="Incli_X">X�����̌X��</param>
	/// <param name="Incli_Y">Y�����̌X��</param>
	/// <param name="Coord_X">�擾�������l��X���W</param>
	/// <param name="Coord_Y">�擾�������l��Y���W</param>
	/// <returns>�w�肵���l�̍���</returns>
	float Waveret_2Dim(float Incli_X, float Incli_Y, float Coord_X, float Coord_Y);

	/// <summary>xorshift�^������</summary>
	/// <param name="v">�K���Ȓl</param>
	/// <returns>�����_���ȃn�b�V���l</returns>
	int xorshift32(int v);

	//�������擾����֐�
	//������ : �Ώۂ̒l ������ : ���߂�ŏ��̌� ��O���� : ���߂�ő�̌�
	//���� : �ꌅ�ڂ�0, �񌅖ڂ�1�Ƃ����悤�Ɏw�肵�Ȃ���΂Ȃ�Ȃ�
	//�߂�l : �؂蔲�����l
	int GetDigits(int value, int m, int n);

	//�����_�ȉ��̎w�肵�����𐮐��l�Ŏ擾����֐�
	int GetFraction(float value, int m);

	/// <summary>�f���𐶐�������֐�</summary>
	/// <param name="article">�����������</param>
	/// <returns>�������ꂽ�f����vector</returns>
	std::vector<int> MakePrime(int article);

	/// <summary>2�_�Ԃ̋�����Ԃ��֐�</summary>
	/// <returns>����</returns>
	float GetDistance(const Transform& tr1, const Transform& tr2);
	float GetDistance(const XMFLOAT3& tr1, const XMFLOAT3& tr2);

	/// <summary>�����m�̋�����Ԃ�</summary>
	/// <param name="tr1">���g��3�������W</param>
	/// <param name="tr2">�Ώۂ�3�������W</param>
	/// <param name="radius1">���g�̔��a</param>
	/// <param name="radius2">�Ώۂ̔��a</param>
	/// <returns>����(�Փ˂��Ă����0���Ԃ�)</returns>
	float GetDistanceSphere(const XMFLOAT3& tr1, const XMFLOAT3& tr2, float radius1, float radius2);

	/// <summary>x = 0��y = 1,x = 1��y = 0�ɂȂ�O���֐�</summary>
	/// <param name="Coord">�擾������x�̈ʒu</param>
	/// <returns>�擾����y�̒l</returns>
	float Func_Cubic(float Coord);

	/// <summary>���_��ʂ�C�ӎ���]</summary>
	/// <param name="pos">�ړ������������̂̈ʒu</param>
	/// <param name="rot">��]�p�x(�x���@)</param>
	/// <param name="axis">���_�ȊO�̎��ƂȂ�ʒu�x�N�g��</param>
	/// <param name="end">���̂�����[�̈ʒu�x�N�g��(���w��Ō��_�ɂȂ�)</param>
	void ArbRotationAxis(XMFLOAT3* pos, float rot, XMVECTOR axis, const XMVECTOR& end = {0,0,0,0});

	/// <summary>���_��ʂ�C�ӎ���]</summary>
	/// <param name="pos">�ړ������������̂̈ʒu</param>
	/// <param name="rad">��]�p�x(���W�A��)</param>
	/// <param name="axis">���_�ȊO�̎��ƂȂ�ʒu�x�N�g��</param>
	/// <param name="end">���̂�����[�̈ʒu�x�N�g��(���w��Ō��_�ɂȂ�)</param>
	void ArbRotationAxisR(XMFLOAT3* pos, float rad, XMVECTOR axis, const XMVECTOR& end = { 0,0,0,0 });

	/// <summary>�l��0~�w�肵���l�ɍ��킹��֐�</summary>
	/// <param name="axis">�����������l�̃|�C���^</param>
	/// <param name="value">�����������l�̍ő�l</param>
	void FixValue(float* axis, int value);

	//�摜��Transform�̈ʒu���s�N�Z���ɕϊ�
	XMFLOAT3 TransformToPixel(const XMFLOAT3& transform);
	//�摜�̃s�N�Z����Transform�ɕϊ�
	XMFLOAT3 PixelToTransform(const XMFLOAT3& pixel);
};