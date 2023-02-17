#include<memory>
#include<DirectXMath.h>
#include"hommingmissile.h"
#include"dx11mathutil.h"
#include"CModel.h"
#include"CDirectInput.h"
#include"ModelMgr.h"
#include "BoundingSphere.h"

#include	<iostream>
#define	DebugLog(b)  std::cout<<b<<std::endl;

constexpr float  BULLETSPEED = 5.0f;
constexpr uint32_t BULLETLIFE = 60;

bool HomingMissile::Init() {
	bool sts = true;
	//�s�񏉊���
	DX11MtxIdentity(m_mtx);
	return sts;
}

void HomingMissile::Draw() {
	//���f���`��
	if (m_sts == HOMINGMISSILESTATUS::LIVE) {
		m_pmodel->Draw(m_mtx);
	}
}

void HomingMissile::Finalize() {}

void HomingMissile::Update()
{
	m_life--;
	if (m_life > 0)
	{
		//�^�[�Q�b�g�Ɍ����Ĉʒu���X�V
		bool sts = Update(m_target, 5.0f);
		if (sts) {
			m_sts = HOMINGMISSILESTATUS::DEAD;
		}
		else if (!sts) {
			m_sts = HOMINGMISSILESTATUS::LIVE;
		}
	}
	else {
		m_sts = HOMINGMISSILESTATUS::DEAD;
	}
	
}

//�z�[�~���O�ƓG�Ƃ̂����蔻��
void HomingMissile::EnemyCollision(DirectX::XMFLOAT3 targetRpos)
{
	erpos = targetRpos; 

	ehpos = 7.8; //Enemy.x�̔��a
	hhpos = 7.8; //Homming.x�̔��a

	/*����͂���*/
	//h�L�[�����������̍��W�łƂ��Ă�
	float lengthl;
	DX11p2pLength(erpos, m_pos, lengthl);
	if (lengthl <= (ehpos+hhpos))
	{
		DebugLog("��������");
	}
	
	//DebugLog(m_pos.x);
	//DebugLog(m_pos.y);
	DebugLog(lengthl);
}


///
///��̃x�N�g������N�H�[�^�j�I�����쐬���� 
///
DirectX::XMFLOAT4 HomingMissile::RotationArc(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, float& d)
{
	DirectX::XMFLOAT3 Axis;//��
	DirectX::XMFLOAT4 q;//���N�H�[�^�j�I��


	//���K���i�O���ł͐��K���ς݁j
	//D3DXVec3Normalaize(&v0,&v0);
	//D3DXVec3Normalaize(&v1,&v1);

	DX11Vec3Cross(Axis, v0, v1);

	DX11Vec3Dot(d, v0, v1);
	//�^�[�Q�b�g�̕����ƖڕW�Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���P�𒴂���ꍇ������i���������̌덷�̂��߁j
	if (d > 1.0)d = 1.0;
	//�^�[�Q�b�g�̕����ƖڕW�Ǝ��@�̕������قƂ�ǈ�v�����ꍇ�A���ς̒l���[�P�𒴂���ꍇ������i���������̌덷�̂��߁j
	if (d <= -1.0)d -= 1.0f;//�␳���Ă��

	float s = (float)sqrtf((1 + d) * 2);
	if (s <= FLT_EPSILON) {
		//DX11QtIdtentity(q);//�^�[�Q�b�g��ǂ��z����
		float radian = acos(d);

		q.x = Axis.x * sin(radian / 2.0f);
		q.y = Axis.y * sin(radian / 2.0f);
		q.z = Axis.z * sin(radian / 2.0f);
		q.w = cos(radian / 2.0f);

	}
	else {
		q.x = Axis.x / s;
		q.y = Axis.y / s;
		q.z = Axis.z / s;
		q.w = s / 2;
	}
	return q;
}

//------------------------------------------
//�~�T�C���̃f�[�^���X�V����
//�i�^�[�Q�b�g�Ƃ̋�����lng��菬�����Ȃ�����true��������
//--------------------------------------------

bool HomingMissile::Update(const DirectX::XMFLOAT3 TargetPosition, float l) {
	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;//�^�[�Q�b�g�����̎p��

	DirectX::XMFLOAT3 TargetVector;

	//���݈ʒu����^�[�Q�b�g�����ւ̃x�N�g�������߂�
	TargetVector.x = TargetPosition.x - m_pos.x;
	TargetVector.y = TargetPosition.y - m_pos.y;
	TargetVector.z = TargetPosition.z - m_pos.z;

	DX11Vec3Length(TargetVector, lng);

	if (lng <= l)
	{
		return true;
	}

	DirectX::XMFLOAT3 ZDir = m_Velocity;//�~�T�C���̑��x�x�N�g���@Z����

	DX11Vec3Normalize(TargetVector, TargetVector);//���K��
	DX11Vec3Normalize(ZDir, ZDir);//���K��

	float Dot;//2�{�̃x�N�g�̓��ϒl�i���݂̐i�s�����ƃ^�[�Q�b�g�����j
	TargetQt = RotationArc(ZDir, TargetVector, Dot);//2�{�̃x�N�g������ׂ��p�x�ƃN�H�[�^�j�I�������߂�
	float angleDiff = acosf(Dot);//���W�A���p�x

	/*
	���̕��������w�I�Ȏ�_�����\�[�X(roattionArc�ɂ���������)
	�P�j�p�x�̔���2�̃x�N�g���̊O�ς͌���Ȃ��������Ȃ�i�덷�𐶂ށj
	�Q�j�^�[�Q�b�g�̕����Ǝ����̕������w�ǈ�v�����ꍇ�A�Ȃ��Ȃ̒l���P�𒴂���ꍇ������


	float Dot;

	DX11Vec3Dot(Dot,TargetVector, ZDir);//�^�[�Q�b�g�̕����Ǝ����̕����̊p�x��
	//�^�[�Q�b�g�̕����Ǝ����̕������w�ǈ�v�����{�ρA�Ȃ��Ȃ̒l���P�𒴂���ꍇ������

	if (Dot > 1.0f)Dot = 1.0f;//�␳���Ă��
	//�^�[�Q�b�g�̕����Ǝ����̒l���w�ǈ�v�����ꍇ�A���ς̒l���[�P�������ꍇ������
	if (Dot < -1.0f)Dot = -1.0f;
	float angleDiff = acos(Dot);//���W�A���p�x

	//�^�[�Q�b�g�����������N�H�[�^�j�I�����v�Z����
	DirectX::XMFLOAT3 Axis;
	DX11Vec3Cross(Axis, ZDir, TargetVector);

	//y������]���Ƃ����N�H�[�^�j�I������]������

	*/

	//�~�T�C�̎p�������肷��
	if (m_AddRotMax >= angleDiff) {
		//1�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���������ꍇ
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		//�P�t���[���ōX�V�ł���p�x�������ݎp���ƃ^�[�Q�b�g�����������p�x���傫���ꍇ
		float t = m_AddRotMax / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);

	}
	//�N�H�[�^�j�I�����s��ɂ���
	//���݂̎p�����N�H�[�^�j�I���ɂ���
	DX11MtxFromQt(m_mtx, m_Quaternion);

	//�~�T�C���̐i�s�����ɃX�s�[�h���|���Ĉړ��ʁi���x�x�N�g�������߂�j
	m_Velocity.x = m_mtx._31 * m_speed;
	m_Velocity.y = m_mtx._32 * m_speed;
	m_Velocity.z = m_mtx._33 * m_speed;

	//�ʒu���X�V�i���x�x�N�g���ōX�V�j
	m_pos.x += m_Velocity.x;
	m_pos.y += m_Velocity.y;
	m_pos.z += m_Velocity.z;

	//�ʒu�s��Ɉʒu���W���Z�b�g
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	return sts;
}
