#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "gameobject.h"
#include "dx11mathutil.h"
#include "enemy.h"

//�O���錾
class CModel;

//�e�X�e�[�^�X
enum class HOMINGMISSILESTATUS {
	LIVE,
	DEAD
};

class HomingMissile : public GameObject {
private:
	HOMINGMISSILESTATUS m_sts;	//�X�e�[�^�X
	CModel* m_pmodel;			//3D�X�e�[�^�X
	DirectX::XMFLOAT3 m_direction;	//���˕���
	float m_speed = 2.0f;			//�X�s�[�h
	uint32_t m_life = 240;			//����
	XMFLOAT3 pos;
	XMFLOAT3 erpos; //�z�[�~���O�Ŏg��Enemy���[�J�����W

	float ehpos;	//Enemy�̔��a
	float hhpos;	//Homming�̔��a

	DirectX::XMFLOAT3	m_Velocity;		//�~�T�C���̌��ݑ��x
	DirectX::XMFLOAT4	m_Quaternion;	//���݂̎p����\���N�H�[�^�j�I��
	float				m_AddRotMax = DirectX::XM_PI * 2.0f / 180.0f; //�ő��]�p�x

	DirectX::XMFLOAT3	m_target;		//�z�[�~���O����^�[�Q�b�g�ʒu

	DirectX::XMFLOAT4 RotationArc(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, float& d);

	bool Update(const DirectX::XMFLOAT3 TargetPosition, float I);

public:

	Enemy* g_enemy;

	bool Init();
	void Draw();
	void Update();
	void Update(XMFLOAT3 target);
	void Finalize();
	void EnemyCollision(DirectX::XMFLOAT3 targetRpos);

	XMFLOAT3 GetPos()
	{
		return pos;
	}

	void SetModel(CModel* p) {
		m_pmodel = p;
	}

	CModel* GetModel() {
		return m_pmodel;
	}

	void SetVelocity(DirectX::XMFLOAT3 vel) {
		m_Velocity = vel;
	}
	
	void SetInitailQuaternion(DirectX::XMFLOAT4X4 mtx)
	{
		DX11GetQtfromMatrix(mtx, m_Quaternion);		//�s�񂩂�N�H�[�^�j�I���𐶐�
	}

	bool isLive() {
		if (m_sts == HOMINGMISSILESTATUS::LIVE) {
			return true;
		}
		else{
			return false;
		}
	}

	void SetTarget(DirectX::XMFLOAT3 targetpos) 
	{
		m_target = targetpos;
	}

	void SetInitialPos(float x,float y, float z)
	{
		m_pos = { x,y,z };
	}

};