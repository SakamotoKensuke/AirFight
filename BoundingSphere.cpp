//*****************************************************************************
//!	@file	CBoundingSphere.cpp
//!	@brief	
//!	@note	���E���̂��߂̃N���X
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	"BoundingSphere.h"

//=============================================================================
//!	@fn		CalculateBoundingSphere
//!	@brief	���b�V���I�u�W�F�N�g����o�E���f�B���O�X�t�B�A�̃f�[�^���쐬����
//!	@param	���b�V���I�u�W�F�N�g
//!	@param	�f�o�C�X�I�u�W�F�N�g
//!	@retval	�Ȃ�
//==============================================================================
void BoundingSphere::CalculateBoundingSphere(std::vector<XMFLOAT3>& vertices){

	HITDATA			atari;					// �����蔻��f�[�^�i�[�\����

	float			minx, miny, minz;		// �ŏ��l�i�w�C�x�C�y�j
	float			maxx, maxy, maxz;		// �ő�l�i�w�C�x�A�y�j
	float			cx, cy, cz;				// ���S���W

	// �w�x�y���ꂼ��ɂ��āA�ő�l�A�ŏ��l�����߂�
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

	cx = (maxx + minx) / 2;		// ���̒��S���W�����߂�
	cy = (maxy + miny) / 2;		// ���̒��S���W�����߂�
	cz = (maxz + minz) / 2;		// ���̒��S���W�����߂�

	float distance;
	float maxdistance = 0;

	// �S���_�ƒ��S���W�Ƃ̋��������߂�
	for(auto v:vertices)
	{
		// 2�_�Ԃ̋��������߂�
		distance = (v.x - cx)*(v.x - cx) + (v.y - cy)*(v.y - cy) + (v.z - cz)*(v.z - cz);
		if (maxdistance<distance) maxdistance = distance;
	}
	// �ő唼�a���v�Z
	maxdistance = sqrtf(maxdistance);
	atari.minx = minx;						// �o�E���f�B���O�{�b�N�X�̂w�̍ŏ��l
	atari.miny = miny;						// �o�E���f�B���O�{�b�N�X�̂x�̍ŏ��l
	atari.minz = minz;						// �o�E���f�B���O�{�b�N�X�̂y�̍ŏ��l
	atari.maxx = maxx;						// �o�E���f�B���O�{�b�N�X�̂w�̍ő�l
	atari.maxy = maxy;						// �o�E���f�B���O�{�b�N�X�̂x�̍ő�l
	atari.maxz = maxz;						// �o�E���f�B���O�{�b�N�X�̂y�̍ő�l
	atari.cx = cx;							// ���E���̒��S���Z�b�g
	atari.cy = cy;
	atari.cz = cz;
	atari.r = maxdistance;					// ���E���̔��a���Z�b�g

	m_hitdata = atari;
}
//******************************************************************************
//	End of file.
//******************************************************************************