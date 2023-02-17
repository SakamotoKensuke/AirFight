#pragma once

#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "gameobject.h"
#include "dx11mathutil.h"
#include "enemy.h"

//前方宣言
class CModel;

//弾ステータス
enum class HOMINGMISSILESTATUS {
	LIVE,
	DEAD
};

class HomingMissile : public GameObject {
private:
	HOMINGMISSILESTATUS m_sts;	//ステータス
	CModel* m_pmodel;			//3Dステータス
	DirectX::XMFLOAT3 m_direction;	//発射方向
	float m_speed = 2.0f;			//スピード
	uint32_t m_life = 240;			//寿命
	XMFLOAT3 pos;
	XMFLOAT3 erpos; //ホーミングで使うEnemyローカル座標

	float ehpos;	//Enemyの半径
	float hhpos;	//Hommingの半径

	DirectX::XMFLOAT3	m_Velocity;		//ミサイルの現在速度
	DirectX::XMFLOAT4	m_Quaternion;	//現在の姿勢を表すクォータニオン
	float				m_AddRotMax = DirectX::XM_PI * 2.0f / 180.0f; //最大回転角度

	DirectX::XMFLOAT3	m_target;		//ホーミングするターゲット位置

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
		DX11GetQtfromMatrix(mtx, m_Quaternion);		//行列からクォータニオンを生成
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