#pragma once
#include	<assimp\Importer.hpp>
#include	<DirectXMath.h>

using namespace DirectX;

struct PLANE {
	float a, b, c, d;
};

void DX11MakeWorldMatrix(XMFLOAT4X4& mat, XMFLOAT3& angle, XMFLOAT3& trans);
void DX11Vec3MulMatrix(DirectX::XMFLOAT3& outvec, const DirectX::XMFLOAT3& invec, const DirectX::XMFLOAT4X4& mat);
void DX11Vec3Cross(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec1, DirectX::XMFLOAT3& invec2);
void DX11Vec3Dot(float& dot, const DirectX::XMFLOAT3& invec1, const DirectX::XMFLOAT3& invec2);
void DX11Vec3Normalize(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec);
void DX11MtxIdentity(DirectX::XMFLOAT4X4& mat);
void DX11GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt);
void DX11QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle);
void DX11QtMul(XMFLOAT4& outqt, const XMFLOAT4& qt1, const XMFLOAT4& qt2);
void DX11MtxFromQt(XMFLOAT4X4& outmtx, const XMFLOAT4& qt);
void DX11QtNormalize(XMFLOAT4& inqt, XMFLOAT4& outqt);
void DX11Vec4Length(XMFLOAT4& vec, float& length);
void DX11p2pLength(const XMFLOAT3& p1, const XMFLOAT3& p2, float& length);
void DX11MtxTranslation(const XMFLOAT3& trans, XMFLOAT4X4& matrix);
void DX11QtSlerp(const XMFLOAT4& fromqt, const XMFLOAT4& toqt, float t, XMFLOAT4& ansqt);
void DX11QtIdentity(XMFLOAT4& qt);
void DX11Vec3Length(XMFLOAT3& vec, float& length);
void DX11MtxRotationAxis(const XMFLOAT3& axis, float angle, XMFLOAT4X4& outmtx);
void DX11MtxMultiply(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& p1mtx, const XMFLOAT4X4& p2mtx);
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix);
void IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[]);
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[], PLANE* plane);
bool IsInFrustum(
	const XMFLOAT3& pos,					// BS中心座標
	float radius,							// 半径
	const XMFLOAT4X4& matrix,				// ビュー変換行列＊プロジェクション変換行列 
	float ans[],							// 平面との距離
	bool  canseeflag[],						// フラスタム平面とＢＳの関係
	PLANE* plane);							// 平面方程式;
void GetPlanefromProjectionmatrix(PLANE* p, const XMFLOAT4X4& matrix);
void GetPlanefromProjectionmatrixWithNormalize(PLANE* p, const XMFLOAT4X4& matrix);
void DX11MtxInverse(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& mtx);
void DX11MtxTranspose(XMFLOAT4X4& outmtx, const XMFLOAT4X4& inmtx);
void DX11MtxRotationX(float angle, XMFLOAT4X4& outmtx);
void DX11MtxRotationY(float angle, XMFLOAT4X4& outmtx);
void DX11MtxRotationZ(float angle, XMFLOAT4X4& outmtx);
void DX11MtxScale(float sx, float sy, float sz, XMFLOAT4X4& outmtx);
float haltonseq(unsigned int no, unsigned int base);
XMFLOAT4X4 DX11MtxaiToDX(aiMatrix4x4& aimtx);

void DX11MtxRotation(float _x, float _y, float _z, XMFLOAT4X4& outmtx);