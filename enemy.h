#pragma once
#include <random>
#include "gameobject.h"
#include "CModel.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"

class Enemy :public GameObject
{
private:
	CModel *m_model;
	Enemy* m_enemy;

	//���
	enum class STATUS
	{
		S1_MOVE_FLY = 0,
		S1_MOVE_BOMB,
	};

	enum class ACTION
	{
		ACT_LEFT = 0,
		ACT_RIGHT,
		ACT_UP,
		ACT_DOWN,
		ACT_ROLLP,
		ACT_ROLLM,
		ACT_STRAIGHT,

		MAX_ACTION
	};

	float m_speed = 0.01;	//�X�s�[�h

	XMFLOAT3 m_angle;	//��]�p

	std::mt19937 m_mt;	//���������p
	std::mt19937 m_mt2;	

	ACTION	m_action;	//�A�N�V�������
	unsigned int m_actionKeepTime;	//�A�N�V�����J�E���^

public:
	XMFLOAT3 m_gEnemypos;

public:
	bool Init();

	void Draw();

	void DrawWithAxis();

	void Update();

	void Finalize();

	void AutoPilot();

	//void SetDirection(DirectX::XMFLOAT4X4 mtx) {
	//	m_mtx = mtx;
	//	m_gEnemypos = DirectX::XMFLOAT3(mtx._41, mtx._42, mtx._43);
	//}

	// �ʒu���擾
	DirectX::XMFLOAT3 *GetPos() {
		return &m_pos;
	}

	CModel* GetModel() 
	{
		return m_model;
	}

	void SetPos(DirectX::XMFLOAT3 pos)
	{
		m_pos = pos;
	}

	void SetModel(CModel* pmodel) {
		m_model = pmodel;
	}
};

