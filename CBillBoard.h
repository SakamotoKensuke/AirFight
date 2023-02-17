#pragma once
#include	<directxmath.h>
#include	<d3d11.h>
#include	"DX11util.h"
#include	"Shader.h"
/*----------------------------------------------------------------------

	ビルボードクラス	

-----------------------------------------------------------------------*/

class CBillBoard{
	DirectX::XMFLOAT4X4			m_mtx;			// ビルボード用の行列
	float						m_x=0;			// ビルボードの位置座標
	float						m_y=0;					
	float						m_z=0;
	float						m_XSize=100.0f;		// ビルボードのＸサイズ
	float						m_YSize=100.0f;		// ビルボードのＹサイズ
	DirectX::XMFLOAT4			m_Color= DirectX::XMFLOAT4(1,1,1,1);	// 頂点カラー値
	ID3D11ShaderResourceView* 	m_srv = nullptr;			// Shader Resourceviewテクスチャ
	ID3D11Buffer*				m_vbuffer = nullptr;		// 頂点バッファ
	ID3D11BlendState*			m_pBlendStateSrcAlpha = nullptr;
	ID3D11BlendState*			m_pBlendStateOne = nullptr;
	ID3D11BlendState*			m_pBlendStateDefault = nullptr;
	ID3D11VertexShader*			m_pVertexShader = nullptr;	// 頂点シェーダー入れ物
	ID3D11PixelShader*			m_pPixelShader = nullptr;	// ピクセルシェーダー入れ物
	ID3D11InputLayout*			m_pVertexLayout = nullptr;	// 頂点フォーマット定義
	ID3D11Resource*				m_res = nullptr;

	// 頂点フォーマット
	struct MyVertex{
		float				x,y,z;
		DirectX::XMFLOAT4	color;
		float				tu,tv;
	};

	MyVertex				m_Vertex[4];			// ビルボードの頂点座標
	DirectX::XMFLOAT2		m_uv[4];				// テクスチャ座標

private:
	// ビルボードの頂点座標を計算
	void CalcVertex();
	// ビルボード用の行列を生成
	void CalcBillBoardMatrix(const DirectX::XMFLOAT4X4& cameramat);
	// 描画
	void Draw();
	// ソースアルファを設定する
	void SetBlendStateSrcAlpha();
	// ブレンドステートを生成する
	void CreateBlendStateSrcAlpha();
	// 加算合成を設定する
	void SetBlendStateOne();
	// ブレンドステート（加算合成）を生成する
	void CreateBlendStateOne();
	// デフォルトのブレンドステートを設定する
	void SetBlendStateDefault();
	// デフォルトのブレンドステートを生成する
	void CreateBlendStateDefault();
	// UV座標をセットする

public:
	void SetUV(DirectX::XMFLOAT2 uv[]) {
		m_uv[0] = uv[0];
		m_uv[1] = uv[1];
		m_uv[2] = uv[2];
		m_uv[3] = uv[3];
		CalcVertex();
	}
	CBillBoard():m_x(0),m_y(0),m_z(0),m_srv(nullptr){
	}

	bool Init(float x, float y, float z, float xsize, float ysize, DirectX::XMFLOAT4 color){
		m_x = x;
		m_y = y;
		m_z = z;
		m_XSize = xsize;
		m_YSize = ysize;
		m_Color = color;

		// デバイス取得
		ID3D11Device* dev = GetDX11Device();
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devcontext = GetDX11DeviceContext();

		// 頂点データの定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		unsigned int numElements = ARRAYSIZE(layout);

		// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
		bool sts = CreateVertexShader(dev,
			"shader/vsbillboard.hlsl",
			"main",
			"vs_4_0",
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
			dev,							// デバイスオブジェクト
			"shader/psbillboard.hlsl", 
			"main", 
			"ps_4_0",
			&m_pPixelShader);

		if (!sts) {
			MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
			return false;
		}

		CalcVertex();						// ビルボード用の頂点データ作成	

		CreateBlendStateSrcAlpha();			// アルファブレンディング用ブレンドステート生成
		CreateBlendStateOne();				// 加算合成用のブレンドステート生成
		CreateBlendStateDefault();			// デフォルトのブレンドステート生成

		return true;
	}

	void Dispose() {
		if (m_vbuffer != nullptr) {
			m_vbuffer->Release();
			m_vbuffer = nullptr;
		}

		if (m_srv != nullptr) {
			m_srv->Release();
			m_srv = nullptr;
		}

		if (m_pBlendStateOne != nullptr) {
			m_pBlendStateOne->Release();
			m_pBlendStateOne = nullptr;
		}

		if (m_pBlendStateSrcAlpha != nullptr) {
			m_pBlendStateSrcAlpha->Release();
			m_pBlendStateSrcAlpha = nullptr;
		}

		if (m_pBlendStateDefault != nullptr) {
			m_pBlendStateDefault->Release();
			m_pBlendStateDefault = nullptr;
		}

		if (m_pPixelShader != nullptr) {
			m_pPixelShader->Release();
			m_pPixelShader = nullptr;
		}

		if (m_pVertexShader != nullptr) {
			m_pVertexShader->Release();
			m_pVertexShader = nullptr;
		}

		if (m_pVertexLayout != nullptr) {
			m_pVertexLayout->Release();
			m_pVertexLayout = nullptr;
		}
	}

	// デストラクタ
	virtual ~CBillBoard(){
		Dispose();
	}

	// ＵＶ座標をセットする
	void SetUV(float u[],float v[]){
		m_Vertex[0].tu = u[0];
		m_Vertex[0].tv = v[0];

		m_Vertex[1].tu = u[1];
		m_Vertex[1].tv = v[1];

		m_Vertex[2].tu = u[2];
		m_Vertex[2].tv = v[2];

		m_Vertex[3].tu = u[3];
		m_Vertex[3].tv = v[3];

		CalcVertex();						// ビルボード用の頂点データ作成	
	}

	// 位置を指定
	void SetPosition(float x,float y,float z);

	// ビルボードを描画
	void DrawBillBoard(const DirectX::XMFLOAT4X4& cameramat);

	// ビルボード描画加算合成
	void DrawBillBoardAdd(const DirectX::XMFLOAT4X4& cameramat);

	// ビルボードをZ軸を中心にして回転させて描画
	void DrawRotateBillBoard(const DirectX::XMFLOAT4X4 &cameramat, float radian);
	
	// サイズをセット
	void SetSize(float x, float y);

	// カラーをセット
	void SetColor(DirectX::XMFLOAT4 col);

	//	テクスチャ読み込み
	bool LoadTexTure(const char* filename);

};