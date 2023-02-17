#include	<assimp\Importer.hpp>
#include	"DX11mathutil.h"
#include	"memory.h"

using namespace DirectX;

/*------------------------
ワールド変換行列を作る
--------------------------*/
void DX11MakeWorldMatrix(XMFLOAT4X4& inoutmat, XMFLOAT3& angle, XMFLOAT3& trans)
{
	ALIGN16 XMMATRIX mat,matx, maty, matz, mattrans;

	matx = XMMatrixRotationX((angle.x*XM_PI) / 180.0f);
	maty = XMMatrixRotationY((angle.y*XM_PI) / 180.0f);
	matz = XMMatrixRotationZ((angle.z*XM_PI) / 180.0f);
	mattrans = XMMatrixTranslation(trans.x, trans.y, trans.z);

	mat = matx*maty*matz*mattrans;

	XMStoreFloat4x4(&inoutmat, mat);
}

/*------------------------
３次元ベクトル×行列
--------------------------*/
void DX11Vec3MulMatrix(DirectX::XMFLOAT3& outvec, const DirectX::XMFLOAT3& invec, const DirectX::XMFLOAT4X4& mat) {

	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMVECTOR inv;
	ALIGN16 XMVECTOR outv;

	mtx = XMLoadFloat4x4(&mat);
	inv = XMLoadFloat3(&invec);

	outv = XMVector3TransformCoord(inv, mtx);

	XMStoreFloat3(&outvec,outv);
}

/*------------------------
外積を計算する
--------------------------*/
void DX11Vec3Cross(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec1, DirectX::XMFLOAT3& invec2) {

	ALIGN16 XMVECTOR inv1;
	ALIGN16 XMVECTOR inv2;
	ALIGN16 XMVECTOR outv;

	inv1 = XMLoadFloat3(&invec1);
	inv2 = XMLoadFloat3(&invec2);

	outv = XMVector3Cross(inv1, inv2);

	XMStoreFloat3(&outvec, outv);
}

/*------------------------
内積を計算する
--------------------------*/
void DX11Vec3Dot(float& dot , const DirectX::XMFLOAT3& invec1, const DirectX::XMFLOAT3& invec2) {

	ALIGN16 XMVECTOR inv1;
	ALIGN16 XMVECTOR inv2;
	ALIGN16 XMVECTOR outv;

	inv1 = XMLoadFloat3(&invec1);
	inv2 = XMLoadFloat3(&invec2);

	outv = XMVector3Dot(inv1, inv2);

	XMFLOAT3 outfloat;

	XMStoreFloat3(&outfloat, outv);

	dot = outfloat.x;
}


/*------------------------
正規化する
--------------------------*/
void DX11Vec3Normalize(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec) {

	ALIGN16 XMVECTOR inv;
	ALIGN16 XMVECTOR outv;

	inv = XMLoadFloat3(&invec);

	outv = DirectX::XMVector3Normalize(inv);

	XMStoreFloat3(&outvec, outv);
}

/*------------------------
単位行列にする
--------------------------*/
void DX11MtxIdentity(DirectX::XMFLOAT4X4& mat) {

	ALIGN16 XMMATRIX mtx;

	mtx = XMMatrixIdentity();

	XMStoreFloat4x4(&mat, mtx);
}

/*------------------------
行列からクオータニオンを生成する
--------------------------*/
void DX11GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt) {
	
	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMVECTOR outvec;
	
	mtx = XMLoadFloat4x4(&mat);

	outvec = XMQuaternionRotationMatrix(mtx);

	XMStoreFloat4(&qt, outvec);
}

/*------------------------
指定軸を中心に回転したクオータニオンを生成する
--------------------------*/
void DX11QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle) {

	ALIGN16 XMVECTOR outqt;
	ALIGN16 XMVECTOR axisvec;

	axisvec = XMLoadFloat4(&axis);

	outqt = XMQuaternionRotationAxis(axisvec, (angle*XM_PI) / 180.0f);

	XMStoreFloat4(&qt, outqt);
}

/*------------------------
クオータニオンの掛け算
--------------------------*/
void DX11QtMul(XMFLOAT4& outqt,const XMFLOAT4& qt1,const XMFLOAT4& qt2) {
	ALIGN16 XMVECTOR qtvec1;
	ALIGN16 XMVECTOR qtvec2;
	ALIGN16 XMVECTOR outqtvec;

	qtvec1 = XMLoadFloat4(&qt1);
	qtvec2 = XMLoadFloat4(&qt2);

	outqtvec = XMQuaternionMultiply(qtvec1, qtvec2);

	XMStoreFloat4(&outqt, outqtvec);
}

/*------------------------
クオータニオンから行列を生成
--------------------------*/
void DX11MtxFromQt(XMFLOAT4X4& outmtx, const XMFLOAT4& qt){

	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMVECTOR qtvec;
	qtvec = XMLoadFloat4(&qt);

	mtx = XMMatrixRotationQuaternion(qtvec);

	XMStoreFloat4x4(&outmtx, mtx);

}

/*------------------------
　　転置する
--------------------------*/
void DX11MtxTranspose(XMFLOAT4X4& outmtx, const XMFLOAT4X4& inmtx) {

	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMMATRIX inmmtx;
	inmmtx = XMLoadFloat4x4(&inmtx);

	mtx = XMMatrixTranspose(inmmtx);

	XMStoreFloat4x4(&outmtx, mtx);

}



/*------------------------
クオータニオンのノーマライズ
--------------------------*/
void DX11QtNormalize(XMFLOAT4& inqt,XMFLOAT4& outqt) {

	ALIGN16 XMVECTOR qtoutvec;
	ALIGN16 XMVECTOR qtvec;
	qtvec = XMLoadFloat4(&inqt);

	qtoutvec = XMQuaternionNormalize(qtvec);

	XMStoreFloat4(&outqt, qtoutvec);

}

/*------------------------
ベクトルの長さを求める
--------------------------*/
void DX11Vec4Length(XMFLOAT4& vec,float& length) {

	ALIGN16 XMVECTOR outvec;
	ALIGN16 XMVECTOR invec;
	XMFLOAT4 l;

	invec = XMLoadFloat4(&vec);

	outvec = XMVector3Length(invec);

	XMStoreFloat4(&l, outvec);

	length = l.x;
}

/*------------------------
ベクトルの長さを求める
--------------------------*/
void DX11Vec3Length(XMFLOAT3& vec, float& length) {

	ALIGN16 XMVECTOR outvec;
	ALIGN16 XMVECTOR invec;
	XMFLOAT4 l;

	l.x = vec.x;
	l.y = vec.y;
	l.z = vec.z;
	l.w = 0.0f;

	invec = XMLoadFloat4(&l);

	outvec = XMVector3Length(invec);

	XMStoreFloat4(&l, outvec);

	length = l.x;
}

/*------------------------
２点間の距離を求める
--------------------------*/
void DX11p2pLength(const XMFLOAT3& p1, const XMFLOAT3& p2,float& length) {

	XMFLOAT4 vec4;

	vec4.x = p1.x - p2.x;
	vec4.y = p1.y - p2.y;
	vec4.z = p1.z - p2.z;
	vec4.w = 1.0f;

	DX11Vec4Length(vec4, length);
}

/*------------------------
平行移動行列を作成する
--------------------------*/
void DX11MtxTranslation(const XMFLOAT3& trans,XMFLOAT4X4& matrix) {
	ALIGN16 XMMATRIX mattrans;

	mattrans = XMMatrixTranslation(trans.x, trans.y, trans.z);

	XMStoreFloat4x4(&matrix, mattrans);
}

/*------------------------
スラープ補間
--------------------------*/
void DX11QtSlerp(const XMFLOAT4& fromqt, const XMFLOAT4& toqt, float t, XMFLOAT4& ansqt) {

	ALIGN16 XMVECTOR infromqt;
	ALIGN16 XMVECTOR intoqt;
	ALIGN16 XMVECTOR qt;

	infromqt = XMLoadFloat4(&fromqt);
	intoqt = XMLoadFloat4(&toqt);

	qt = XMQuaternionSlerp(infromqt, intoqt, t);

	XMStoreFloat4(&ansqt, qt);
}

/*------------------------
  単位クオータニオンにする
--------------------------*/
void DX11QtIdentity(XMFLOAT4& qt) {

	ALIGN16 XMVECTOR outqt;

	outqt = XMQuaternionIdentity();

	XMStoreFloat4(&qt, outqt);

}

/*------------------------
指定軸で回転する行列を作成する（angleは、度数法）
--------------------------*/
void DX11MtxRotationAxis(const XMFLOAT3& axis, float angle ,XMFLOAT4X4& outmtx) {

	ALIGN16 XMVECTOR vaxis;
	ALIGN16 XMMATRIX mtx;

	// 軸を同時座標に変換
	XMFLOAT4 axis4 = {axis.x,axis.y,axis.z,0};
	vaxis = XMLoadFloat4(&axis4);

	// 度数法をラジアンに変換
	angle = (angle * XM_PI) / 180.0f;

	// 指定軸を中心に回転する行列を作成する
	mtx = XMMatrixRotationAxis(vaxis,angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
行列の掛け算
--------------------------*/
void DX11MtxMultiply(XMFLOAT4X4& ansmtx,const XMFLOAT4X4& p1mtx, const XMFLOAT4X4& p2mtx) {

	ALIGN16 XMMATRIX mat1, mat2, matans;

	mat1 = XMLoadFloat4x4(&p1mtx);
	mat2 = XMLoadFloat4x4(&p2mtx);

	matans = XMMatrixMultiply(mat1, mat2);

	XMStoreFloat4x4(&ansmtx, matans);
}

/*------------------------
逆行列を求める
--------------------------*/
void DX11MtxInverse(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& mtx){

	ALIGN16 XMMATRIX mat, matans;

	mat = XMLoadFloat4x4(&mtx);

	matans = DirectX::XMMatrixInverse(nullptr, mat);

	XMStoreFloat4x4(&ansmtx, matans);
}



//==============================================================================
//!	@fn		IsInFrustum
//!	@brief	フラスタム内かどうかをチェック
//!	@param　位置座標（ワールド）　　	　	
//!	@param　ビュー変換行列＊プロジェクション変換行列
//!	@retval	true フラスタム内　false フラスタム外
//==============================================================================
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix) {

	PLANE plane[6];

	GetPlanefromProjectionmatrix(plane, matrix);		// 行列から６平面を取り出す

	for (int i = 0; i < 6; i++) {
		float ans = plane[i].a*pos.x + plane[i].b*pos.y + plane[i].c*pos.z + plane[i].d;
		if (ans < 0) {
			return false;
		}
	}
	return true;
}

//==============================================================================
//!	@fn		IsInFrustum
//!	@brief	フラスタム内かどうかをチェック
//!	@param　位置座標（ワールド）　　	　	
//!	@param　ビュー変換行列＊プロジェクション変換行列
//!	@param　６平面との距離
//!	@retval	なし
//==============================================================================
void IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[]) {

	PLANE plane[6];

	GetPlanefromProjectionmatrixWithNormalize(plane, matrix);

	for (int i = 0; i < 6; i++) {
		ans[i] = plane[i].a*pos.x + plane[i].b*pos.y + plane[i].c*pos.z + plane[i].d;
	}
}

//==============================================================================
//!	@fn		IsInFrustum
//!	@brief	フラスタム内かどうかをチェック
//!	@param　位置座標（ワールド）　　	　	
//!	@param　ビュー変換行列＊プロジェクション変換行列
//!	@retval	true フラスタム内　false フラスタム外
//==============================================================================
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix, float ans[], PLANE* plane) {

	GetPlanefromProjectionmatrixWithNormalize(plane, matrix);

	for (int i = 0; i < 6; i++) {
		ans[i] = plane[i].a*pos.x + plane[i].b*pos.y + plane[i].c*pos.z + plane[i].d;
		if (ans[i] < 0) {
			return false;
		}
	}

	return true;
}

//==============================================================================
//!	@fn		IsInFrustum
//!	@brief	フラスタム内かどうかをチェック
//!	@param　位置座標（ワールド）　　	　	
//!	@param　ＢＳ半径　　	　	
//!	@param　ビュー変換行列＊プロジェクション変換行列
//!	@param　フラスタム平面との距離
//!	@param　ＢＳが見えるかどうかフラグ
//!	@param　平面の方程式
//!	@retval	true フラスタム内　false フラスタム外
//==============================================================================
bool IsInFrustum(
	const XMFLOAT3& pos,					// BS中心座標
	float radius,							// 半径
	const XMFLOAT4X4& matrix,				// ビュー変換行列＊プロジェクション変換行列 
	float ans[],							// 平面との距離
	bool  canseeflag[],						// フラスタム平面とＢＳの関係
	PLANE* plane) {							// 平面方程式

	bool flag = true;

	// 行列からフラスタム６平面の方程式（正規化済み）を取り出す
	GetPlanefromProjectionmatrixWithNormalize(plane, matrix);

	// フラスタム６平面との距離を求める
	for (int i = 0; i < 6; i++) {
		// 平面の方程式に値を当てはめて距離を求める
		ans[i] = plane[i].a*pos.x + plane[i].b*pos.y + plane[i].c*pos.z + plane[i].d;

		if (ans[i] + radius< 0) {		// ＢＳの半径を考慮してフラスタム内かををチェック
			canseeflag[i] = false;
			flag = false;
		}
		else {
			canseeflag[i] = true;
		}
	}

	return flag;
}

//==============================================================================
//!	@fn		GetPlanefromProjectionmatrix
//!	@brief	プロジェクション変換行列からフラスタムの６平面を取り出す
//!	@param　平面の方程式
//!	@param　ビュー変換行列＊プロジェクション変換行列
//==============================================================================
void GetPlanefromProjectionmatrix(PLANE* p, const XMFLOAT4X4& matrix) {
	// left
	p[0].a = matrix._14 + matrix._11;
	p[0].b = matrix._24 + matrix._21;
	p[0].c = matrix._34 + matrix._31;
	p[0].d = matrix._44 + matrix._41;

	// right
	p[1].a = matrix._14 - matrix._11;
	p[1].b = matrix._24 - matrix._21;
	p[1].c = matrix._34 - matrix._31;
	p[1].d = matrix._44 - matrix._41;

	// bottom
	p[2].a = matrix._14 + matrix._12;
	p[2].b = matrix._24 + matrix._22;
	p[2].c = matrix._34 + matrix._32;
	p[2].d = matrix._44 + matrix._42;

	// top
	p[3].a = matrix._14 - matrix._12;
	p[3].b = matrix._24 - matrix._22;
	p[3].c = matrix._34 - matrix._32;
	p[3].d = matrix._44 - matrix._42;

	// near
	p[4].a = matrix._13;
	p[4].b = matrix._23;
	p[4].c = matrix._33;
	p[4].d = matrix._43;

	// far
	p[5].a = matrix._14 - matrix._13;
	p[5].b = matrix._24 - matrix._23;
	p[5].c = matrix._34 - matrix._33;
	p[5].d = matrix._44 - matrix._43;
}

//==============================================================================
//!	@fn		GetPlanefromProjectionmatrixWithNormalize
//!	@brief	プロジェクション変換行列からフラスタムの６平面を取り出す(正規化行う)
//!	@param　平面の方程式
//!	@param　ビュー変換行列＊プロジェクション変換行列
//==============================================================================
void GetPlanefromProjectionmatrixWithNormalize(PLANE* p, const XMFLOAT4X4& matrix) {

	// left
	p[0].a = matrix._14 + matrix._11;
	p[0].b = matrix._24 + matrix._21;
	p[0].c = matrix._34 + matrix._31;
	p[0].d = matrix._44 + matrix._41;

	// right
	p[1].a = matrix._14 - matrix._11;
	p[1].b = matrix._24 - matrix._21;
	p[1].c = matrix._34 - matrix._31;
	p[1].d = matrix._44 - matrix._41;

	// bottom
	p[2].a = matrix._14 + matrix._12;
	p[2].b = matrix._24 + matrix._22;
	p[2].c = matrix._34 + matrix._32;
	p[2].d = matrix._44 + matrix._42;

	// top
	p[3].a = matrix._14 - matrix._12;
	p[3].b = matrix._24 - matrix._22;
	p[3].c = matrix._34 - matrix._32;
	p[3].d = matrix._44 - matrix._42;

	// near
	p[4].a = matrix._13;
	p[4].b = matrix._23;
	p[4].c = matrix._33;
	p[4].d = matrix._43;

	// far
	p[5].a = matrix._14 - matrix._13;
	p[5].b = matrix._24 - matrix._23;
	p[5].c = matrix._34 - matrix._33;
	p[5].d = matrix._44 - matrix._43;

	float length;

	// 正規化
	for (int i = 0; i < 6; i++) {
		length = sqrtf(p[i].a*p[i].a + p[i].b*p[i].b + p[i].c*p[i].c);

		p[i].a = p[i].a / length;
		p[i].b = p[i].b / length;
		p[i].c = p[i].c / length;
		p[i].d = p[i].d / length;
	}
}

/*------------------------
X軸回転行列を求める
--------------------------*/
void DX11MtxRotationX(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * XM_PI) / 180.0f;

	// X軸を中心に回転する行列を作成する
	mtx = XMMatrixRotationX(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
  Y軸回転行列を求める
--------------------------*/
void DX11MtxRotationY(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * XM_PI) / 180.0f;

	// Y軸を中心に回転する行列を作成する
	mtx = XMMatrixRotationY(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
   Z軸回転行列を求める
--------------------------*/
void DX11MtxRotationZ(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * XM_PI) / 180.0f;

	// 指定軸を中心に回転する行列を作成する
	mtx = XMMatrixRotationZ(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
 スケーリング行列を求める
--------------------------*/
void DX11MtxScale(float sx, float sy, float sz,XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	mtx = XMMatrixScaling(sx,sy,sz);

	XMStoreFloat4x4(&outmtx, mtx);
}


/*------------------------
ハルトン数列を計算
入力
unsigned int no : 何番目か
unsined base : 基底

戻り値
　　ハルトン数列値
--------------------------*/
float haltonseq(unsigned int no, unsigned int base) {

	unsigned int i = no;
	float result = 0;
	float f = 1;

	while (i > 0) {
		f = f / base;
		result = result + f * (i % base);
		i = i / base;
	}

	return result;
}

/*------------------------
  assimp mtx => DX mtx
--------------------------*/
XMFLOAT4X4 DX11MtxaiToDX(aiMatrix4x4& aimtx) {
	XMFLOAT4X4 mtx;
	/*
		mtx._11 = aimtx.a1;
		mtx._12 = aimtx.a2;
		mtx._13 = aimtx.a3;
		mtx._14 = aimtx.a4;

		mtx._21 = aimtx.b1;
		mtx._22 = aimtx.b2;
		mtx._23 = aimtx.b3;
		mtx._24 = aimtx.b4;

		mtx._31 = aimtx.c1;
		mtx._32 = aimtx.c2;
		mtx._33 = aimtx.c3;
		mtx._34 = aimtx.c4;

		mtx._41 = aimtx.d1;
		mtx._42 = aimtx.d2;
		mtx._43 = aimtx.d3;
		mtx._44 = aimtx.d4;
	*/

	mtx._11 = aimtx.a1;
	mtx._21 = aimtx.a2;
	mtx._31 = aimtx.a3;
	mtx._41 = aimtx.a4;

	mtx._12 = aimtx.b1;
	mtx._22 = aimtx.b2;
	mtx._32 = aimtx.b3;
	mtx._42 = aimtx.b4;

	mtx._13 = aimtx.c1;
	mtx._23 = aimtx.c2;
	mtx._33 = aimtx.c3;
	mtx._43 = aimtx.c4;

	mtx._14 = aimtx.d1;
	mtx._24 = aimtx.d2;
	mtx._34 = aimtx.d3;
	mtx._44 = aimtx.d4;

	return mtx;
}


//回転行列を求める

void DX11MtxRotation(float _x, float _y, float _z, XMFLOAT4X4 & outmtx)
{
	ALIGN16 XMMATRIX mtx;

	// X軸を中心に回転する行列を作成する
	mtx = XMMatrixRotationRollPitchYaw((_x * XM_PI) / 180.0f, (_y * XM_PI) / 180.0f, (_z * XM_PI) / 180.0f);


	XMStoreFloat4x4(&outmtx, mtx);
}