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

	// ���ʂ�`�悷��
	ComPtr<ID3D11Buffer> m_pVertexBuffer;       // ���_�o�b�t�@
	ComPtr<ID3D11Buffer> m_pIndexBuffer;        // �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D11VertexShader> m_pVertexShader; // ���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // �s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout> m_pVertexLayout;  // ���_���C�A�E�g

		//�@���ʃ`�b�v
	Vertex m_v[4] = {
		{DirectX::XMFLOAT3(-0.5f,0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f) },
		{DirectX::XMFLOAT3(0.5f,0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f) },
		{DirectX::XMFLOAT3(-0.5f,-0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f) },
		{DirectX::XMFLOAT3(0.5f,-0.5f,0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f) }
	};

	XMFLOAT4X4 m_mtx;
};