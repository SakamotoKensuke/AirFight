#define NOMINMAX
#include	<windows.h>
#include	"CModel.h"
#include	"Shader.h"
#include	"CDirectxGraphics.h"
#include	"DX11Settransform.h"
#include	"ModelData.h"

bool CModel::Init(const char* filename, const char* vsfile, const char* psfile,std::string texfolder) {

	bool sts;

	std::string fname(filename);

	sts = m_assimpfile.Load(texfolder,fname);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	m_filetype = eASSIMPFILE;

	// 頂点データの定義（アニメーション対応）
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(device,
		vsfile,
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

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
		device,		// デバイスオブジェクト
		psfile,
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}

	return true;
}

void CModel::Uninit() {

	// 頂点シェーダー解放
	if (m_pVertexShader) {
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}

	// ピクセルシェーダー解放
	if (m_pPixelShader) {
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}

	// 頂点レイアウト解放
	if (m_pVertexLayout) {
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}

// アニメーション更新
void CModel::Update(
	unsigned int animno,
	const XMFLOAT4X4& mtxworld){

}

void CModel::Draw(XMFLOAT4X4& mtxworld) {

	ID3D11DeviceContext*	devcontext;			// デバイスコンテキスト
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(m_pVertexLayout);
	// 頂点シェーダーをセット
	devcontext->VSSetShader(m_pVertexShader, nullptr, 0);
	// ピクセルシェーダーをセット
	devcontext->PSSetShader(m_pPixelShader, nullptr, 0);
	// 描画
	m_assimpfile.Draw(devcontext,mtxworld);
}