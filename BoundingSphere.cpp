//*****************************************************************************
//!	@file	CBoundingSphere.cpp
//!	@brief	
//!	@note	境界球のためのクラス
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"BoundingSphere.h"

//=============================================================================
//!	@fn		CalculateBoundingSphere
//!	@brief	メッシュオブジェクトからバウンディングスフィアのデータを作成する
//!	@param	メッシュオブジェクト
//!	@param	デバイスオブジェクト
//!	@retval	なし
//==============================================================================
void BoundingSphere::CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices){

	HITDATA			atari;					// あたり判定データ格納構造体

	float			minx, miny, minz;		// 最小値（Ｘ，Ｙ，Ｚ）
	float			maxx, maxy, maxz;		// 最大値（Ｘ，Ｙ、Ｚ）
	float			cx, cy, cz;				// 中心座標

	// ＸＹＺそれぞれについて、最大値、最小値を求める
	minx = maxx = vertices[0].x;
	miny = maxy = vertices[0].y;
	minz = maxz = vertices[0].z;

	for (auto v: vertices)
	{
		if (minx >= v.x) minx = v.x;
		if (miny >= v.y) miny = v.y;
		if (minz >= v.z) minz = v.z;
		if (maxx <= v.x) maxx = v.x;
		if (maxy <= v.y) maxy = v.y;
		if (maxz <= v.z) maxz = v.z;
	}

	cx = (maxx + minx) / 2;		// 球の中心座標を求める
	cy = (maxy + miny) / 2;		// 球の中心座標を求める
	cz = (maxz + minz) / 2;		// 球の中心座標を求める

	float distance;
	float maxdistance = 0;

	// 全頂点と中心座標との距離を求める
	for(auto v:vertices)
	{
		// 2点間の距離を求める
		distance = (v.x - cx)*(v.x - cx) + (v.y - cy)*(v.y - cy) + (v.z - cz)*(v.z - cz);
		if (maxdistance<distance) maxdistance = distance;
	}
	// 最大半径を計算
	maxdistance = sqrtf(maxdistance);
	atari.minx = minx;						// バウンディングボックスのＸの最小値
	atari.miny = miny;						// バウンディングボックスのＹの最小値
	atari.minz = minz;						// バウンディングボックスのＺの最小値
	atari.maxx = maxx;						// バウンディングボックスのＸの最大値
	atari.maxy = maxy;						// バウンディングボックスのＹの最大値
	atari.maxz = maxz;						// バウンディングボックスのＺの最大値
	atari.cx = cx;							// 境界球の中心をセット
	atari.cy = cy;
	atari.cz = cz;
	atari.r = maxdistance;					// 境界球の半径をセット

	m_hitdata = atari;
}
//******************************************************************************
//	End of file.
//******************************************************************************