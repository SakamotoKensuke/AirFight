//*****************************************************************************
//!	@file	CBoundingSphere.h
//!	@brief	境界球クラス
//!	@note	
//!	@author
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<DirectXMath.h>
#include	"Sphere.h"
#include	"DX11util.h"
#include	"dx11mathutil.h"
#include	"DX11Settransform.h"

//-----------------------------------------------------------------------------
// マクロ宣言
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
struct	HITDATA{
	float	minx, miny, minz;			// バウンディングボックス最小値	
	float	maxx, maxy, maxz;			// バウンディングボックス最大値
	float	cx, cy, cz;					// バウンディングスフィア中心座標
	float	r;							// バウンディングスフィア半径
};

class BoundingSphere{
private:
	HITDATA				m_hitdata{};				// 当たり判定用データ
	DirectX::XMFLOAT3	m_wpos{};					// ワールド空間上の位置座標
	DirectX::XMFLOAT4X4	m_matrix{};				// 境界球の描画用行列
	Sphere				m_spheremesh;			// 球メッシュ
	
	void CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices);
	void CreateSphereMesh(float r,DirectX::XMFLOAT4 color){
		// バウンディングスフィアを表す球メッシュを生成
		bool sts = m_spheremesh.Init(r,				// 半径
			50,								// 水平方向の分割数
			50,								// 垂直方向の分割数
			color,
			GetDX11Device());				// デバイスオブジェクト
		if (!sts) {
			MessageBox(nullptr,"CSphere init error","error",MB_OK);
		}
	}
public:
	BoundingSphere(){
	}

	~BoundingSphere(){
		Exit();
	}

	void Init(std::vector<XMFLOAT3>& vertices,XMFLOAT4 color){
		CalculateBoundingSphere(vertices);			// ヒットデータの計算
		CreateSphereMesh(m_hitdata.r,color);		// 球メッシュを作成する
	}

	void UpdatePosition(const DirectX::XMFLOAT4X4& mtx){
		DirectX::XMFLOAT3 pos(m_hitdata.cx, m_hitdata.cy, m_hitdata.cz);

		// 行列×ベクトル（ワールド座標に変換）
		DX11Vec3MulMatrix(m_wpos, pos, mtx);

		m_matrix = mtx;
		m_matrix._41 = m_wpos.x;
		m_matrix._42 = m_wpos.y;
		m_matrix._43 = m_wpos.z;
	}

	void Exit(){
	}

	void Draw(){
		ID3D11DeviceContext* device;
		device = GetDX11DeviceContext();
		// ワールド変換行列
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_matrix);
		m_spheremesh.Draw(device);
	}

	float GetR() const{
		return m_hitdata.r;
	}

	const DirectX::XMFLOAT3& GetWPos(){
		return m_wpos;
	}

	const HITDATA& GetHitdata(){
		return m_hitdata;
	}
};

//******************************************************************************
//	End of file.
//******************************************************************************