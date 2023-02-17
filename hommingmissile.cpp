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
	//行列初期化
	DX11MtxIdentity(m_mtx);
	return sts;
}

void HomingMissile::Draw() {
	//モデル描画
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
		//ターゲットに向けて位置を更新
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

//ホーミングと敵とのあたり判定
void HomingMissile::EnemyCollision(DirectX::XMFLOAT3 targetRpos)
{
	erpos = targetRpos; 

	ehpos = 7.8; //Enemy.xの半径
	hhpos = 7.8; //Homming.xの半径

	/*判定はある*/
	//hキーを押した時の座標でとられてる
	float lengthl;
	DX11p2pLength(erpos, m_pos, lengthl);
	if (lengthl <= (ehpos+hhpos))
	{
		DebugLog("あたった");
	}
	
	//DebugLog(m_pos.x);
	//DebugLog(m_pos.y);
	DebugLog(lengthl);
}


///
///二つのベクトルからクォータニオンを作成する 
///
DirectX::XMFLOAT4 HomingMissile::RotationArc(DirectX::XMFLOAT3 v0, DirectX::XMFLOAT3 v1, float& d)
{
	DirectX::XMFLOAT3 Axis;//軸
	DirectX::XMFLOAT4 q;//軸クォータニオン


	//正規化（外部では正規化済み）
	//D3DXVec3Normalaize(&v0,&v0);
	//D3DXVec3Normalaize(&v1,&v1);

	DX11Vec3Cross(Axis, v0, v1);

	DX11Vec3Dot(d, v0, v1);
	//ターゲットの方向と目標と時機の方向がほとんど一致した場合、内積の値が１を超える場合がある（浮動小数の誤差のため）
	if (d > 1.0)d = 1.0;
	//ターゲットの方向と目標と時機の方向がほとんど一致した場合、内積の値がー１を超える場合がある（浮動小数の誤差のため）
	if (d <= -1.0)d -= 1.0f;//補正してやる

	float s = (float)sqrtf((1 + d) * 2);
	if (s <= FLT_EPSILON) {
		//DX11QtIdtentity(q);//ターゲットを追い越した
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
//ミサイルのデータを更新する
//（ターゲットとの距離がlngより小さくなったらtrueをかえす
//--------------------------------------------

bool HomingMissile::Update(const DirectX::XMFLOAT3 TargetPosition, float l) {
	bool sts = false;
	float lng;

	DirectX::XMFLOAT4 TargetQt;//ターゲット方向の姿勢

	DirectX::XMFLOAT3 TargetVector;

	//現在位置からターゲット方向へのベクトルを求める
	TargetVector.x = TargetPosition.x - m_pos.x;
	TargetVector.y = TargetPosition.y - m_pos.y;
	TargetVector.z = TargetPosition.z - m_pos.z;

	DX11Vec3Length(TargetVector, lng);

	if (lng <= l)
	{
		return true;
	}

	DirectX::XMFLOAT3 ZDir = m_Velocity;//ミサイルの速度ベクトル　Z方向

	DX11Vec3Normalize(TargetVector, TargetVector);//正規化
	DX11Vec3Normalize(ZDir, ZDir);//正規化

	float Dot;//2本のベクトの内積値（現在の進行方向とターゲット方向）
	TargetQt = RotationArc(ZDir, TargetVector, Dot);//2本のベクトルから為す角度とクォータニオンを求める
	float angleDiff = acosf(Dot);//ラジアン角度

	/*
	この部分が数学的な弱点を持つソース(roattionArcにおきかえる)
	１）角度の薄い2つのベクトルの外積は限りなく小さくなり（誤差を生む）
	２）ターゲットの方向と時期の方向が殆ど一致した場合、ない席の値が１を超える場合がある


	float Dot;

	DX11Vec3Dot(Dot,TargetVector, ZDir);//ターゲットの方向と時期の方向の角度差
	//ターゲットの方向と時期の方向が殆ど一致した倍委、ない席の値が１を超える場合がある

	if (Dot > 1.0f)Dot = 1.0f;//補正してやる
	//ターゲットの方向と時期の値が殆ど一致した場合、内積の値がー１を下回る場合がある
	if (Dot < -1.0f)Dot = -1.0f;
	float angleDiff = acos(Dot);//ラジアン角度

	//ターゲット方向を向くクォータニオンを計算する
	DirectX::XMFLOAT3 Axis;
	DX11Vec3Cross(Axis, ZDir, TargetVector);

	//y軸を回転軸としたクォータニオンを回転させる

	*/

	//ミサイの姿勢を決定する
	if (m_AddRotMax >= angleDiff) {
		//1フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が小さい場合
		DX11QtMul(m_Quaternion, m_Quaternion, TargetQt);
	}
	else {
		//１フレームで更新できる角度よりも現在姿勢とターゲット方向を向く角度が大きい場合
		float t = m_AddRotMax / angleDiff;

		DirectX::XMFLOAT4 toqt;
		DX11QtMul(toqt, m_Quaternion, TargetQt);

		DX11QtSlerp(m_Quaternion, toqt, t, m_Quaternion);

	}
	//クォータニオンを行列にする
	//現在の姿勢をクォータニオンにする
	DX11MtxFromQt(m_mtx, m_Quaternion);

	//ミサイルの進行方向にスピードを掛けて移動量（速度ベクトルを求める）
	m_Velocity.x = m_mtx._31 * m_speed;
	m_Velocity.y = m_mtx._32 * m_speed;
	m_Velocity.z = m_mtx._33 * m_speed;

	//位置を更新（速度ベクトルで更新）
	m_pos.x += m_Velocity.x;
	m_pos.y += m_Velocity.y;
	m_pos.z += m_Velocity.z;

	//位置行列に位置座標をセット
	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;

	return sts;
}
