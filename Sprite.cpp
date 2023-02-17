#include "Sprite.h"
#include "Application.h"

bool Sprite::Create(std::string _texpath)
{
	ID3D11Device* dev;
	ID3D11DeviceContext* devcontext;

	dev = GetDX11Device();
	devcontext = GetDX11DeviceContext();

	bool sts;
	sts = CreateSRVfromFile(_texpath.c_str(), dev, devcontext, &m_tex, &m_srv);

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexBuffer error(stage_34)", "error", MB_OK);
	}

	sts = CreateVertexBuffer(dev, sizeof(Vertex), 4, m_v, &m_pVertexBuffer);

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexbuffer error(stage_41)", "error", MB_OK);
	}

	//頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",	0,DXGI_FORMAT_R32G32B32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,			0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		/*{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
	};

	unsigned int numElements = ARRAYSIZE(layout);

	sts = CreateVertexShader(dev,
		"shader/vs.hlsl",
		"main",
		"vs_5_0",
		layout,
		numElements,
		&m_pVertexShader,
		&m_pVertexLayout
	);

	if (!sts)
	{
		MessageBox(nullptr, "CreateVertexShader error(stage_65)", "error", MB_OK);
	}

	sts = CreatePixelShader(
		dev,
		"shader/pstexcol.hlsl",
		"main",
		"ps_5_0",
		&m_pPixelShader);

	if (!sts)
	{
		MessageBox(nullptr, "CreatePixelShader error(stage_76)", "error", MB_OK);
	}

	return sts;
}

void Sprite::Destroy()
{
}


void Sprite::Draw()
{
	//printf("isDraw\n");
	ID3D11DeviceContext* devcontect;
	devcontect = GetDX11DeviceContext();

	unsigned int stride = sizeof(Vertex);

	unsigned offset = 0;
	
	CreateVertexBuffer(GetDX11Device(), sizeof(Vertex), 4, m_v, &m_pVertexBuffer);

	devcontect->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	
	devcontect->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	devcontect->IASetInputLayout(m_pVertexLayout.Get());

	devcontect->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	devcontect->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	
	ID3D11DeviceContext* devcomtext;
	devcomtext = GetDX11DeviceContext();


	DirectX::XMFLOAT4X4 mtx;
	DirectX::XMFLOAT4X4 fmtx;

	DX11MtxScale(scale.x, scale.y, scale.z,mtx);
	fmtx = mtx;
	//DirectX::XMFLOAT3 trans = { pos.x,pos.y,pos.z };
	//if (isBillBoard)
	//{
	//	// 逆行列作成
	//	XMMATRIX inverseView = XMMatrixTranspose(DX11SetTransform::GetInstance()->View());
	//	inverseView = XMMatrixInverse(nullptr, inverseView);
	//	// できた逆行列の平行移動成分を消した
	//	inverseView.r[3].m128_f32[0] = 0.0f;
	//	inverseView.r[3].m128_f32[1] = 0.0f;
	//	inverseView.r[3].m128_f32[2] = 0.0f;
	//	XMStoreFloat4x4(&mtx,inverseView);
	//}

		DX11MtxRotation(rot.x, rot.y, rot.z, mtx);

	DX11MtxMultiply(fmtx, fmtx, mtx);	
	
	DX11MtxTranslation(pos, mtx);

	DX11MtxMultiply(fmtx, fmtx, mtx);
	
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, fmtx);

	devcomtext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());

	devcomtext->Draw(4, 0);

}

void Sprite::Finalize()
{
	Destroy();
}

void Sprite::SetPosition(float _x, float _y, float _z)
{
	//printf("%f,%f,%f\n", _x, _y, _z);
	
	if (isBillBoard == false)
	{
		_x = _x / static_cast<float>(Application::CLIENT_WIDTH);
		_y = _y / static_cast<float>(Application::CLIENT_HEIGHT);
	}

	pos.x = _x;
	pos.y = _y;
	pos.z = _z;
}

void Sprite::SetScale(float _x, float _y, float _z)
{
	//printf("%f,%f,%f\n", _x, _y, _z);
	if (isBillBoard==false)
	{
		_x = _x / static_cast<float>(Application::CLIENT_WIDTH);
		_y = _y / static_cast<float>(Application::CLIENT_HEIGHT);
	}

	scale.x = _x;
	scale.y = _y;
	scale.z = _z;
}

void Sprite::SetRotation(float _x, float _y, float _z)
{
	rot = { _x,_y,_z };
}

void Sprite::SetUV(DirectX::XMINT2 _pos, DirectX::XMINT2 _split)
{
	float _x = 1.0f / (float)_split.x;
	float _y = 1.0f / (float)_split.y;

	m_v[0].m_Tex = { _x * _pos.x, _y * _pos.y };
	m_v[1].m_Tex = { _x * _pos.x, _y * (_pos.y + 1) };
	m_v[2].m_Tex = { _x * (_pos.x + 1), _y * _pos.y };
	m_v[3].m_Tex = { _x * (_pos.x + 1), _y * (_pos.y + 1) };
}

//void Sprite::SetColor(DirectX::XMFLOAT4 _col)
//{
//	color = _col;
//	m_v[0].m_Color = _col;
//	m_v[1].m_Color = _col;
//	m_v[2].m_Color = _col;
//	m_v[3].m_Color = _col;
//}

