#pragma once
#include <wrl/client.h>
#include "vertexproto.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#include "DX11util.h"



class Cpngtexture
{
public:
	bool Init(const char* fileName);
	void Dispoce();

	void Draw();

private:
	ComPtr<ID3D11ShaderResourceView> m_srv;
	ComPtr<ID3D11Resource> m_tex;

	// 平面を描画する
	ComPtr<ID3D11Buffer> m_pVertexBuffer;       // 頂点バッファ
	ComPtr<ID3D11Buffer> m_pIndexBuffer;        // インデックスバッファ
	ComPtr<ID3D11VertexShader> m_pVertexShader; // 頂点シェーダー
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // ピクセルシェーダー
	ComPtr<ID3D11InputLayout> m_pVertexLayout;  // 頂点レイアウト

		//　平面チップ
	Vertex m_v[4] = {
		{DirectX::XMFLOAT3(-0.5f,0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{DirectX::XMFLOAT3(0.5f,0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{DirectX::XMFLOAT3(-0.5f,-0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{DirectX::XMFLOAT3(0.5f,-0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
	};

	XMFLOAT4X4 m_mtx;
};