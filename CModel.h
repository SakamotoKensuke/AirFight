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
	FILETYPE				m_filetype = eASSIMPFILE;		// ファイルタイプ
	ModelData				m_assimpfile;					// assimpfile
	ID3D11VertexShader*     m_pVertexShader = nullptr;		// 頂点シェーダー入れ物
	ID3D11PixelShader*      m_pPixelShader = nullptr;		// ピクセルシェーダー入れ物
	ID3D11InputLayout*      m_pVertexLayout = nullptr;		// 頂点フォーマット定義

	ID3D11ShaderResourceView*	m_texSRV = nullptr;			// テクスチャＳＲＶ

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
