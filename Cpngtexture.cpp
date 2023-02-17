#include "Cpngtexture.h"

bool Cpngtexture::Init(const char* fileName)
{
	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = GetDX11Device();
	devcontext = GetDX11DeviceContext();

	bool sts;
	sts = CreateSRVfromFile(fileName, dev, devcontext, &m_tex, &m_srv);

	if (!sts) {
		MessageBox(nullptr, "load tex error (stage)", "error", MB_OK);
	}

	sts = CreateVertexBuffer(dev, sizeof(Vertex), 4, m_v, &m_pVertexBuffer);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error(stage)", "error", MB_OK);
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = CreateVertexShader(dev,
		"shader/vs.hlsl",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout);

	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error(stage)", "error", MB_OK);
	}

	// ピクセルシェーダーを生成
	sts = CreatePixelShader(      // ピクセルシェーダーオブジェクトを生成
		dev,                      // デバイスオブジェクト
		"shader/pstexcol.hlsl",
		"main",
		"ps_5_0",
		&m_pPixelShader);
	if (!sts) {
		MessageBox(nullptr, "createPixelShader error(stage)", "error", MB_OK);
	}

	return sts;
}

void Cpngtexture::Dispoce()
{

}

void Cpngtexture::Draw()
{
	ID3D11DeviceContext* devcontext;

	devcontext = GetDX11DeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned offset = 0;
	devcontext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);         // トポロジーをセット(旧プリミティブタイプ)
	devcontext->IASetInputLayout(m_pVertexLayout.Get());                                // 頂点レイアウトセット
	devcontext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);                         // 頂点シェーダーセット
	devcontext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);                          // ピクセルシェーダーをセット

	//ID3D11DeviceContext* devcontext;

	devcontext = GetDX11DeviceContext();

	DirectX::XMFLOAT4X4 mtx;
	DirectX::XMFLOAT3 trans = { 0,0,0 };

	DX11MtxTranslation(trans, mtx);

	// ワールド変換行列セット
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtx);

	// テクスチャセット
	devcontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());

	devcontext->Draw(4, 0);   // 描画する頂点数
}
