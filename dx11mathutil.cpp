#include	<assimp\Importer.hpp>
#include	"DX11mathutil.h"
#include	"memory.h"

using namespace DirectX;

/*------------------------
���[���h�ϊ��s������
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
�R�����x�N�g���~�s��
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
�O�ς��v�Z����
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
���ς��v�Z����
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
���K������
--------------------------*/
void DX11Vec3Normalize(DirectX::XMFLOAT3& outvec, DirectX::XMFLOAT3& invec) {

	ALIGN16 XMVECTOR inv;
	ALIGN16 XMVECTOR outv;

	inv = XMLoadFloat3(&invec);

	outv = DirectX::XMVector3Normalize(inv);

	XMStoreFloat3(&outvec, outv);
}

/*------------------------
�P�ʍs��ɂ���
--------------------------*/
void DX11MtxIdentity(DirectX::XMFLOAT4X4& mat) {

	ALIGN16 XMMATRIX mtx;

	mtx = XMMatrixIdentity();

	XMStoreFloat4x4(&mat, mtx);
}

/*------------------------
�s�񂩂�N�I�[�^�j�I���𐶐�����
--------------------------*/
void DX11GetQtfromMatrix(const XMFLOAT4X4& mat, XMFLOAT4& qt) {
	
	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMVECTOR outvec;
	
	mtx = XMLoadFloat4x4(&mat);

	outvec = XMQuaternionRotationMatrix(mtx);

	XMStoreFloat4(&qt, outvec);
}

/*------------------------
�w�莲�𒆐S�ɉ�]�����N�I�[�^�j�I���𐶐�����
--------------------------*/
void DX11QtRotationAxis(XMFLOAT4& qt, XMFLOAT4& axis, float angle) {

	ALIGN16 XMVECTOR outqt;
	ALIGN16 XMVECTOR axisvec;

	axisvec = XMLoadFloat4(&axis);

	outqt = XMQuaternionRotationAxis(axisvec, (angle*XM_PI) / 180.0f);

	XMStoreFloat4(&qt, outqt);
}

/*------------------------
�N�I�[�^�j�I���̊|���Z
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
�N�I�[�^�j�I������s��𐶐�
--------------------------*/
void DX11MtxFromQt(XMFLOAT4X4& outmtx, const XMFLOAT4& qt){

	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMVECTOR qtvec;
	qtvec = XMLoadFloat4(&qt);

	mtx = XMMatrixRotationQuaternion(qtvec);

	XMStoreFloat4x4(&outmtx, mtx);

}

/*------------------------
�@�@�]�u����
--------------------------*/
void DX11MtxTranspose(XMFLOAT4X4& outmtx, const XMFLOAT4X4& inmtx) {

	ALIGN16 XMMATRIX mtx;
	ALIGN16 XMMATRIX inmmtx;
	inmmtx = XMLoadFloat4x4(&inmtx);

	mtx = XMMatrixTranspose(inmmtx);

	XMStoreFloat4x4(&outmtx, mtx);

}



/*------------------------
�N�I�[�^�j�I���̃m�[�}���C�Y
--------------------------*/
void DX11QtNormalize(XMFLOAT4& inqt,XMFLOAT4& outqt) {

	ALIGN16 XMVECTOR qtoutvec;
	ALIGN16 XMVECTOR qtvec;
	qtvec = XMLoadFloat4(&inqt);

	qtoutvec = XMQuaternionNormalize(qtvec);

	XMStoreFloat4(&outqt, qtoutvec);

}

/*------------------------
�x�N�g���̒��������߂�
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
�x�N�g���̒��������߂�
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
�Q�_�Ԃ̋��������߂�
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
���s�ړ��s����쐬����
--------------------------*/
void DX11MtxTranslation(const XMFLOAT3& trans,XMFLOAT4X4& matrix) {
	ALIGN16 XMMATRIX mattrans;

	mattrans = XMMatrixTranslation(trans.x, trans.y, trans.z);

	XMStoreFloat4x4(&matrix, mattrans);
}

/*------------------------
�X���[�v���
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
  �P�ʃN�I�[�^�j�I���ɂ���
--------------------------*/
void DX11QtIdentity(XMFLOAT4& qt) {

	ALIGN16 XMVECTOR outqt;

	outqt = XMQuaternionIdentity();

	XMStoreFloat4(&qt, outqt);

}

/*------------------------
�w�莲�ŉ�]����s����쐬����iangle�́A�x���@�j
--------------------------*/
void DX11MtxRotationAxis(const XMFLOAT3& axis, float angle ,XMFLOAT4X4& outmtx) {

	ALIGN16 XMVECTOR vaxis;
	ALIGN16 XMMATRIX mtx;

	// ���𓯎����W�ɕϊ�
	XMFLOAT4 axis4 = {axis.x,axis.y,axis.z,0};
	vaxis = XMLoadFloat4(&axis4);

	// �x���@�����W�A���ɕϊ�
	angle = (angle * XM_PI) / 180.0f;

	// �w�莲�𒆐S�ɉ�]����s����쐬����
	mtx = XMMatrixRotationAxis(vaxis,angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
�s��̊|���Z
--------------------------*/
void DX11MtxMultiply(XMFLOAT4X4& ansmtx,const XMFLOAT4X4& p1mtx, const XMFLOAT4X4& p2mtx) {

	ALIGN16 XMMATRIX mat1, mat2, matans;

	mat1 = XMLoadFloat4x4(&p1mtx);
	mat2 = XMLoadFloat4x4(&p2mtx);

	matans = XMMatrixMultiply(mat1, mat2);

	XMStoreFloat4x4(&ansmtx, matans);
}

/*------------------------
�t�s������߂�
--------------------------*/
void DX11MtxInverse(XMFLOAT4X4& ansmtx, const XMFLOAT4X4& mtx){

	ALIGN16 XMMATRIX mat, matans;

	mat = XMLoadFloat4x4(&mtx);

	matans = DirectX::XMMatrixInverse(nullptr, mat);

	XMStoreFloat4x4(&ansmtx, matans);
}



//==============================================================================
//!	@fn		IsInFrustum
//!	@brief	�t���X�^�������ǂ������`�F�b�N
//!	@param�@�ʒu���W�i���[���h�j�@�@	�@	
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
//!	@retval	true �t���X�^�����@false �t���X�^���O
//==============================================================================
bool IsInFrustum(const XMFLOAT3& pos, const XMFLOAT4X4& matrix) {

	PLANE plane[6];

	GetPlanefromProjectionmatrix(plane, matrix);		// �s�񂩂�U���ʂ����o��

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
//!	@brief	�t���X�^�������ǂ������`�F�b�N
//!	@param�@�ʒu���W�i���[���h�j�@�@	�@	
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
//!	@param�@�U���ʂƂ̋���
//!	@retval	�Ȃ�
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
//!	@brief	�t���X�^�������ǂ������`�F�b�N
//!	@param�@�ʒu���W�i���[���h�j�@�@	�@	
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
//!	@retval	true �t���X�^�����@false �t���X�^���O
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
//!	@brief	�t���X�^�������ǂ������`�F�b�N
//!	@param�@�ʒu���W�i���[���h�j�@�@	�@	
//!	@param�@�a�r���a�@�@	�@	
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
//!	@param�@�t���X�^�����ʂƂ̋���
//!	@param�@�a�r�������邩�ǂ����t���O
//!	@param�@���ʂ̕�����
//!	@retval	true �t���X�^�����@false �t���X�^���O
//==============================================================================
bool IsInFrustum(
	const XMFLOAT3& pos,					// BS���S���W
	float radius,							// ���a
	const XMFLOAT4X4& matrix,				// �r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s�� 
	float ans[],							// ���ʂƂ̋���
	bool  canseeflag[],						// �t���X�^�����ʂƂa�r�̊֌W
	PLANE* plane) {							// ���ʕ�����

	bool flag = true;

	// �s�񂩂�t���X�^���U���ʂ̕������i���K���ς݁j�����o��
	GetPlanefromProjectionmatrixWithNormalize(plane, matrix);

	// �t���X�^���U���ʂƂ̋��������߂�
	for (int i = 0; i < 6; i++) {
		// ���ʂ̕������ɒl�𓖂Ă͂߂ċ��������߂�
		ans[i] = plane[i].a*pos.x + plane[i].b*pos.y + plane[i].c*pos.z + plane[i].d;

		if (ans[i] + radius< 0) {		// �a�r�̔��a���l�����ăt���X�^�����������`�F�b�N
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
//!	@brief	�v���W�F�N�V�����ϊ��s�񂩂�t���X�^���̂U���ʂ����o��
//!	@param�@���ʂ̕�����
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
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
//!	@brief	�v���W�F�N�V�����ϊ��s�񂩂�t���X�^���̂U���ʂ����o��(���K���s��)
//!	@param�@���ʂ̕�����
//!	@param�@�r���[�ϊ��s�񁖃v���W�F�N�V�����ϊ��s��
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

	// ���K��
	for (int i = 0; i < 6; i++) {
		length = sqrtf(p[i].a*p[i].a + p[i].b*p[i].b + p[i].c*p[i].c);

		p[i].a = p[i].a / length;
		p[i].b = p[i].b / length;
		p[i].c = p[i].c / length;
		p[i].d = p[i].d / length;
	}
}

/*------------------------
X����]�s������߂�
--------------------------*/
void DX11MtxRotationX(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// �x���@�����W�A���ɕϊ�
	angle = (angle * XM_PI) / 180.0f;

	// X���𒆐S�ɉ�]����s����쐬����
	mtx = XMMatrixRotationX(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
  Y����]�s������߂�
--------------------------*/
void DX11MtxRotationY(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// �x���@�����W�A���ɕϊ�
	angle = (angle * XM_PI) / 180.0f;

	// Y���𒆐S�ɉ�]����s����쐬����
	mtx = XMMatrixRotationY(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
   Z����]�s������߂�
--------------------------*/
void DX11MtxRotationZ(float angle, XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	// �x���@�����W�A���ɕϊ�
	angle = (angle * XM_PI) / 180.0f;

	// �w�莲�𒆐S�ɉ�]����s����쐬����
	mtx = XMMatrixRotationZ(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
 �X�P�[�����O�s������߂�
--------------------------*/
void DX11MtxScale(float sx, float sy, float sz,XMFLOAT4X4& outmtx) {

	ALIGN16 XMMATRIX mtx;

	mtx = XMMatrixScaling(sx,sy,sz);

	XMStoreFloat4x4(&outmtx, mtx);
}


/*------------------------
�n���g��������v�Z
����
unsigned int no : ���Ԗڂ�
unsined base : ���

�߂�l
�@�@�n���g������l
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


//��]�s������߂�

void DX11MtxRotation(float _x, float _y, float _z, XMFLOAT4X4 & outmtx)
{
	ALIGN16 XMMATRIX mtx;

	// X���𒆐S�ɉ�]����s����쐬����
	mtx = XMMatrixRotationRollPitchYaw((_x * XM_PI) / 180.0f, (_y * XM_PI) / 180.0f, (_z * XM_PI) / 180.0f);


	XMStoreFloat4x4(&outmtx, mtx);
}