//*****************************************************************************
//!	@file	CBoundingSphere.h
//!	@brief	���E���N���X
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
// �}�N���錾
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
struct	HITDATA{
	float	minx, miny, minz;			// �o�E���f�B���O�{�b�N�X�ŏ��l	
	float	maxx, maxy, maxz;			// �o�E���f�B���O�{�b�N�X�ő�l
	float	cx, cy, cz;					// �o�E���f�B���O�X�t�B�A���S���W
	float	r;							// �o�E���f�B���O�X�t�B�A���a
};

class BoundingSphere{
private:
	HITDATA				m_hitdata{};				// �����蔻��p�f�[�^
	DirectX::XMFLOAT3	m_wpos{};					// ���[���h��ԏ�̈ʒu���W
	DirectX::XMFLOAT4X4	m_matrix{};				// ���E���̕`��p�s��
	Sphere				m_spheremesh;			// �����b�V��
	
	void CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices);
	void CreateSphereMesh(float r,DirectX::XMFLOAT4 color){
		// �o�E���f�B���O�X�t�B�A��\�������b�V���𐶐�
		bool sts = m_spheremesh.Init(r,				// ���a
			50,								// ���������̕�����
			50,								// ���������̕�����
			color,
			GetDX11Device());				// �f�o�C�X�I�u�W�F�N�g
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
		CalculateBoundingSphere(vertices);			// �q�b�g�f�[�^�̌v�Z
		CreateSphereMesh(m_hitdata.r,color);		// �����b�V�����쐬����
	}

	void UpdatePosition(const DirectX::XMFLOAT4X4& mtx){
		DirectX::XMFLOAT3 pos(m_hitdata.cx, m_hitdata.cy, m_hitdata.cz);

		// �s��~�x�N�g���i���[���h���W�ɕϊ��j
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
		// ���[���h�ϊ��s��
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