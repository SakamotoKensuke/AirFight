#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "gameobject.h"
#include "ModelMgr.h"

//前方宣言
class CModel;

//弾ステータス
enum class BULLETSTATUS
{
	LIVE,
	DEAD
};

class Bullet :public GameObject {
private:
	BULLETSTATUS		m_sts;			//ステータス
	CModel*				m_pmodel;		//3Dモデル
	DirectX::XMFLOAT3	m_direction;	//発砲方向
	DirectX::XMFLOAT3   m_initial;		//
	float				m_speed = 2.0f;
	uint32_t			m_life = 60;
public:
	bool Init();

	void Drow();

	void Update();
	
	void Finalize();

	void SetModel(CModel* p) {
		m_pmodel = p;
	}

	void SetDirection(DirectX::XMFLOAT4X4 mtx){
		m_mtx = mtx;
		m_direction = DirectX::XMFLOAT3(mtx._31, mtx._32, mtx._33);
	}

	void SetInitialPos(DirectX::XMFLOAT3 pos) {
		m_pos = pos;
	}

	bool isLive() {
		if (m_sts == BULLETSTATUS::LIVE)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};