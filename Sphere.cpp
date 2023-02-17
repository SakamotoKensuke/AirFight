#include	"Sphere.h"
#include	"dx11mathutil.h"

// �@���x�N�g�����v�Z
// �m�[�}���C�Y
void Sphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal,Normal);
}

// �C���f�b�N�X�f�[�^���쐬
void Sphere::CreateIndex() {

	// �C���f�b�N�X����
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1)*y + x;			// ������W�̃C���f�b�N�X

			Face f;
			// �㔼��
			f.idx[0] = count;
			f.idx[1] = count + 1;
			f.idx[2] = count + 1 + (m_divX + 1);

			m_face.emplace_back(f);

			// ������
			f.idx[0] = count;
			f.idx[1] = count + (m_divX + 1) + 1;
			f.idx[2] = count + (m_divX + 1);

			m_face.emplace_back(f);

		}
	}
}

// ���_�f�[�^���쐬
void Sphere::CreateVertex() {

	float azimuth = 0.0f;			// ���ʊp
	float elevation = 0.0f;			// �p

	XMFLOAT3	Normal;
	// ���ʊp�Ƌp���狅���b�V���̒��_�f�[�^���쐬
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (PI * (float)y) / (float)m_divY;    // �p���Z�b�g
		float r = m_radius*sinf(elevation);				// �p�ɉ��������a���v�Z

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (2 * PI * (float)x) / (float)m_divX;	// ���ʊp���Z�b�g

			Vertex v;
			v.Pos.x = r * cosf(azimuth);
			v.Pos.y = m_radius * cosf(elevation);
			v.Pos.z = r * sinf(azimuth);

			Normalize(v.Pos, Normal);		// �@�����v�Z
			v.Normal = Normal;				// �@�����Z�b�g

			v.Color = m_color;				// ���_�J���[

			m_vertex.emplace_back(v);
		}
	}
}

// �`��
void Sphere::Draw(ID3D11DeviceContext* device) {

	// ���_�o�b�t�@���Z�b�g����
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	device->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	device->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// �C���f�b�N�X�o�b�t�@���Z�b�g
	device->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// �g�|���W�[���Z�b�g�i���v���~�e�B�u�^�C�v�j
	device->IASetInputLayout(m_pVertexLayout.Get());					// ���_���C�A�E�g�Z�b�g

	device->VSSetShader(m_pVertexShader.Get(), nullptr, 0);			// ���_�V�F�[�_�[���Z�b�g
	device->PSSetShader(m_pPixelShader.Get(), nullptr, 0);			// �s�N�Z���V�F�[�_�[���Z�b�g

	device->DrawIndexed(
		static_cast<unsigned int>(m_face.size() * 3),		// �`�悷��C���f�b�N�X���i�ʐ��~�R�j
		0,									// �ŏ��̃C���f�b�N�X�o�b�t�@�̈ʒu
		0);									// ���_�o�b�t�@�̍ŏ�����g��
}

bool Sphere::Init(float r,				// ���a
	int division_horizontal,			// ���������̕�����
	int division_vertical,				// ���������̕�����
	XMFLOAT4 color,
	ID3D11Device* device) {

	bool sts;
		// ��������ۑ�
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// ���a��ۑ�
	m_radius = r;

	// �J���[�l��ۑ�
	m_color = color;

	// ���̃C���f�b�N�X�f�[�^���쐬
	CreateIndex();

	// ���̒��_�f�[�^���쐬
	CreateVertex();

	// ���_�o�b�t�@�쐬
	sts = CreateVertexBufferWrite(
		device,
		sizeof(Vertex),						// �P���_������o�C�g��
		m_vertex.size(),					// ���_��
		m_vertex.data(),					// ���_�f�[�^�i�[�������擪�A�h���X
		m_pVertexBuffer.GetAddressOf());	// ���_�o�b�t�@
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// �C���f�b�N�X�o�b�t�@�쐬
	sts = CreateIndexBuffer(
			device,								// �f�o�C�X�I�u�W�F�N�g
			m_face.size()*3,					// �C���f�b�N�X��
			m_face.data(),						// �C���f�b�N�X�f�[�^�擪�A�h���X
			m_pIndexBuffer.GetAddressOf());		// �C���f�b�N�X�o�b�t�@

	if (!sts) {
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = CreateVertexShader(device,
		"shader/vssphere.hlsl",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = CreatePixelShader(			// �s�N�Z���V�F�[�_�[�I�u�W�F�N�g�𐶐�
		device,							// �f�o�C�X�I�u�W�F�N�g
		"shader/pssphere.hlsl",
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	return true;
}
