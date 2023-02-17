#pragma once
#include	<DirectXMath.h>
#include	"memory.h"
#include	"Shader.h"
#include	"DX11util.h"

class CLight {
	ALIGN16 struct ConstantBufferLight {
		DirectX::XMFLOAT4 LightDirection;
		DirectX::XMFLOAT4 EyePos;
		DirectX::XMFLOAT4 Ambient;
	};

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT
	};
	LightType			m_type;
	DirectX::XMFLOAT3	m_eyepos;
	DirectX::XMFLOAT4	m_lightpos;		// w=0�̎��͕��� w=1�̎��͈ʒu
	ID3D11Buffer*       m_pConstantBufferLight = nullptr;
	DirectX::XMFLOAT4	m_ambient;
public:
	bool Init(DirectX::XMFLOAT3 eyepos,DirectX::XMFLOAT4 lightpos) {
		m_lightpos = lightpos;
		m_eyepos = eyepos;
		m_type = LightType::DIRECTIONAL;

		// �R���X�^���g�o�b�t�@�쐬
		bool sts = CreateConstantBuffer(
			GetDX11Device(),				// �f�o�C�X
			sizeof(ConstantBufferLight),		// �T�C�Y
			&m_pConstantBufferLight);			// �R���X�^���g�o�b�t�@�S
		if (!sts) {
			MessageBox(NULL, "CreateBuffer(constant buffer Light) error", "Error", MB_OK);
			return false;
		}

		Update();

		return true;
	}

	void Update() {
		ConstantBufferLight		cb;

		cb.EyePos.w = 1.0;
		cb.EyePos.x = m_eyepos.x;
		cb.EyePos.y = m_eyepos.y;
		cb.EyePos.z = m_eyepos.z;

		cb.LightDirection.x = m_lightpos.x;
		cb.LightDirection.y = m_lightpos.y;
		cb.LightDirection.z = m_lightpos.z;
		cb.LightDirection.w = m_lightpos.w;

		cb.Ambient = m_ambient;

		GetDX11DeviceContext()->UpdateSubresource(m_pConstantBufferLight, 
			0, 
			nullptr, 
			&cb, 
			0, 0);

		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g�i���_�V�F�[�_�[�p�j
		GetDX11DeviceContext()->VSSetConstantBuffers(4, 1, &m_pConstantBufferLight);
		// �R���X�^���g�o�b�t�@4����3���W�X�^�փZ�b�g(�s�N�Z���V�F�[�_�[�p)
		GetDX11DeviceContext()->PSSetConstantBuffers(4, 1, &m_pConstantBufferLight);

	}

	void Uninit() {
		if (m_pConstantBufferLight) {
			m_pConstantBufferLight->Release();
			m_pConstantBufferLight = nullptr;
		}
	}

	void SetEyePos(DirectX::XMFLOAT3 eyepos) {
		m_eyepos = eyepos;
	}

	void SetLightPos(DirectX::XMFLOAT4 lightpos) {
		m_lightpos = lightpos;
	}

	void SetAmbient(DirectX::XMFLOAT4 amb) {
		m_ambient = amb;
	}
};