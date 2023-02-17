#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include	<wrl/client.h>
#include	"Shader.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout>  m_pVertexLayout;	// ���_���C�A�E�g
	unsigned int		m_divX;					// ���������̕�����
	unsigned int		m_divY;					// ���������̕�����
	float				m_radius;				// ���a
	XMFLOAT4			m_color;				// ���_�J���[

	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// �C���f�b�N�X�f�[�^
	struct Face{
		unsigned int idx[3];
	};

	std::vector<Vertex>		m_vertex;			//�@���_�f�[�^
	std::vector<Face>		m_face;				//	�C���f�b�N�X�f�[�^

	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();
	// ���K��
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
public:
	// �`��
	void Draw(ID3D11DeviceContext* device);

	// ������
	bool Init(float r,						// ���a
		int division_horizontal,			// ���������̕�����
		int division_vertical,				// ���������̕�����
		XMFLOAT4	color,					// ���_�J���[
		ID3D11Device* device);
};