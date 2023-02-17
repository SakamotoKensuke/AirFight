#pragma once

#include <wrl/client.h>
#include <string>

#include "vertexproto.h"
#include "dx11mathutil.h"
#include "DX11Settransform.h"
#include "DX11util.h"


class Sprite
{
	ComPtr<ID3D11ShaderResourceView> m_srv;
	ComPtr<ID3D11Resource> m_tex;

	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	ComPtr<ID3D11InputLayout> m_pVertexLayout;

	//struct Vertex {
	//	DirectX::XMFLOAT3 m_Pos;            //    à íu
	//	DirectX::XMFLOAT3 m_Normal;            //    ñ@ê¸
	//	DirectX::XMFLOAT2 m_Tex;            //    UVç¿ïW
	//	DirectX::XMFLOAT4 m_Color;
	//};

	Vertex m_v[4] = {
		{DirectX::XMFLOAT3(-1.0f,1,0.0f),		//pos
		DirectX::XMFLOAT3(0,1.0f,0),			// NOMAL
		DirectX::XMFLOAT2(0.0f,0.0f),			//uv
		//DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	//color
		},		

		{DirectX::XMFLOAT3(-1.0f,-1,0.0f),	//pos
		DirectX::XMFLOAT3(0,1.0f,0),		// nomal
		DirectX::XMFLOAT2(0.0f,1.0f),		//uv
		//DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	//color
		},

		{DirectX::XMFLOAT3(1.0f,1,0.0f),//pos
		DirectX::XMFLOAT3(0,1.0f,0),		// nomal
		DirectX::XMFLOAT2(1.0f,0.0f),		//uv
		//DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	//color
		},

		{DirectX::XMFLOAT3(1.0f,-1,0.0f),	//pos
		DirectX::XMFLOAT3(0,1.0f,0),		//nomal
		DirectX::XMFLOAT2(1.0f,1.0f),			//uv
		//DirectX::XMFLOAT4(1.0f,1.0f,1.0f,1.0f)	//color
		},
	};

	DirectX::XMFLOAT3 pos = { 0,0,0 };
	DirectX::XMFLOAT3 scale = { 0.1f,0.1f,0 };
	DirectX::XMFLOAT3 rot = { 0,0,0 };
	//DirectX::XMFLOAT4 color = { 1,1,1,1 };
public:
	Sprite(const Sprite&) = delete;
	Sprite& operator=(const Sprite&) = delete;

	Sprite(Sprite&&) = delete;
	Sprite() = default;
	Sprite& operator=(Sprite&) = delete;

	bool Create(std::string _texpath);
	void Destroy();
	void Draw();
	void Finalize();

	void SetPosition(float _x, float _y, float _z);
	void SetScale(float _x, float _y, float _z);
	void SetRotation(float _x, float _y, float _z);
	void SetUV(DirectX::XMINT2 _pos,DirectX::XMINT2 _split);
	//void SetColor(DirectX::XMFLOAT4 _col);

	bool isBillBoard = false;
	DirectX::XMFLOAT3 GetPosition() { return pos; }
};

