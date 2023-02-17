#pragma once
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<d3d11.h>
#include	<vector>
#include	"DX11util.h"
#include	"Shader.h"

using Microsoft::WRL::ComPtr;

class Line {
private:
	ComPtr<ID3D11Buffer>		m_vbuffer;			// 頂点バッファ
	ComPtr<ID3D11PixelShader>	m_ps;				// ピクセルシェーダー
	ComPtr<ID3D11VertexShader>	m_vs;				// 頂点シェーダー
	ComPtr<ID3D11InputLayout>   m_layout;			// 頂点フォーマット定義
	size_t						m_vsize = 0;		// 頂点数
public:
	struct MyVertex {	
		DirectX::XMFLOAT3		vertex;
		DirectX::XMFLOAT4		color;
	};

	Line() {

	}

	~Line() {

	}

	bool Init(const std::vector<MyVertex>& v) {
		m_vsize = v.size();

		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
		bool sts = CreateVertexShader(
			GetDX11Device(),
			"shader/vsline.hlsl",
			"main",
			"vs_5_0",
			layout,
			numElements,
			&m_vs,
			&m_layout);
		if (!sts) {
			MessageBox(nullptr, "CreateVertexShader(Line) error", "error", MB_OK);
			return false;
		}

		// ピクセルシェーダーを生成
		sts = CreatePixelShader(			// ピクセルシェーダーオブジェクトを生成
			GetDX11Device(),				// デバイスオブジェクト
			"shader/psline.hlsl",
			"main",
			"ps_5_0",
			&m_ps);
		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader(Line) error", "error", MB_OK);
			return false;
		}

		// 頂点バッファ生成
		sts = CreateVertexBufferWrite(GetDX11Device(), sizeof(MyVertex), v.size(), (void*)v.data(), &m_vbuffer);
		if (!sts) {
			MessageBox(nullptr, "CreateVertexBufferWrite(Line) error", "error", MB_OK);
			return false;
		}

		return true;
	}

	void Dispose() {
	}

	void SetVertex(std::vector<MyVertex>& v) {

		D3D11_MAPPED_SUBRESOURCE pData;

		HRESULT hr = GetDX11DeviceContext()->Map(m_vbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
		if (SUCCEEDED(hr)) {
			memcpy_s(pData.pData, pData.RowPitch, (void*)(v.data()), sizeof(MyVertex) * v.size());
			GetDX11DeviceContext()->Unmap(m_vbuffer.Get(), 0);
		}

		m_vsize = v.size();
	}

	void Draw() {
		ID3D11DeviceContext* devcontext=GetDX11DeviceContext();

		// 頂点バッファをセットする
		unsigned int stride = sizeof(MyVertex);
		unsigned  offset = 0;
		devcontext->IASetVertexBuffers(0, 1, m_vbuffer.GetAddressOf(), &stride, &offset);

		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);		// トポロジーをセット（旧プリミティブタイプ）
		devcontext->IASetInputLayout(m_layout.Get());			// 頂点レイアウトセット

		devcontext->VSSetShader(m_vs.Get(), nullptr, 0);		// 頂点シェーダーをセット
		devcontext->PSSetShader(m_ps.Get(), nullptr, 0);		// ピクセルシェーダーをセット

		devcontext->Draw(
			m_vsize,							// 頂点数
			0);									// 頂点バッファの最初から使う
	}
};