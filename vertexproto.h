#pragma once
#include	<directxmath.h>
#include	<assimp/Importer.hpp>

typedef	int32_t VertexSuu; // ���_��
struct Vertex {
	DirectX::XMFLOAT3 m_Pos;			//	�ʒu
	DirectX::XMFLOAT3 m_Normal;			//	�@��
	DirectX::XMFLOAT2 m_Tex;			//	UV���W
	int				m_BoneIndex[4];		//	�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
	float			m_BoneWeight[4];	//	�E�F�C�g�l
	int				m_BoneNum;			//	�{�[����
	std::string		m_BoneName[4];		//	�{���̓{�[���C���f�b�N�X�ŊǗ�����ׂ�
};

typedef	int32_t IndexSuu;     // �C���f�b�N�X�̐��B
typedef	uint32_t Index;       // �C���f�b�N�X�B

// �}�e���A��
struct Material {
	float m_Alpha;
	DirectX::XMFLOAT3 m_Diffuse;
	DirectX::XMFLOAT3 m_Specular;
	float	m_SpecularPower;
	DirectX::XMFLOAT3 m_Ambient;
	DirectX::XMFLOAT3 m_Emissive;

	Material()
		: m_Alpha(0),
		m_Diffuse(DirectX::XMFLOAT3(0, 0, 0)),
		m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
		m_SpecularPower(0),
		m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
		m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

struct BONE
{
	//std::string Name;
	unsigned int idx = -1;			// �{�[���z��̃C���f�b�N�X�ԍ�
	aiMatrix4x4 Matrix;				// �����z�u�s��
	aiMatrix4x4 AnimationMatrix;	// �{�[���s��
	aiMatrix4x4 OffsetMatrix;		// �{�[���I�t�Z�b�g�s��
};

struct ConstantBufferBoneMatrix		// �{�[���s��i�[�p�̃R���X�^���g�o�b�t�@
{
	DirectX::XMFLOAT4X4  mBoneMatrix[400];
};

// �萔�o�b�t�@��`�i�}�e���A���j
struct ConstantBufferMaterial {
	DirectX::XMFLOAT4	AmbientMaterial;		// �����̔��˗�
	DirectX::XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
	DirectX::XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
};
