#pragma once
#include	<d3d11.h>
#include	<vector>
#include	<wrl/client.h>
#include	"ModelData.h"

using Microsoft::WRL::ComPtr;

class CModel {
private:
	enum FILETYPE{
		eDATFILE,
		eASSIMPFILE
	};
	FILETYPE				m_filetype = eASSIMPFILE;		// �t�@�C���^�C�v
	ModelData				m_assimpfile;					// assimpfile
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// ���_�V�F�[�_�[���ꕨ
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// �s�N�Z���V�F�[�_�[���ꕨ
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// ���_�t�H�[�}�b�g��`

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// �e�N�X�`���r�q�u

public:
	bool Init(const char* filename,const char* vsfile,const char* psfile, std::string texfolder);
	void Uninit();
	void Update(
		unsigned int animno,
		const DirectX::XMFLOAT4X4& mtxworld);
	void Draw(DirectX::XMFLOAT4X4& mtxworld);

	const ModelData& GetModelData() const{
		return m_assimpfile;
	}
};
