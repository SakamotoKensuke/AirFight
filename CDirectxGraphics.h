#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>

using Microsoft::WRL::ComPtr;

class CDirectXGraphics {
private:
	CDirectXGraphics() {}
	ComPtr<ID3D11Device>			m_lpDevice;					// DIRECT3DDEVICE11デバイス
	ComPtr<ID3D11DeviceContext>		m_lpImmediateContext;		// DIRECT3DDEVICE11デバイスコンテキスト
	ComPtr<IDXGISwapChain>			m_lpSwapChain;				// スワップチェイン

	ComPtr<ID3D11RenderTargetView>	m_lpRenderTargetView;		// レンダーターゲットビュー
	ComPtr<ID3D11Texture2D>			m_depthStencilBuffer;		// Ｚバッファ、ステンシルバッファ
	ComPtr<ID3D11DepthStencilState>	m_depthStencilState;		// Ｚバッファ、ステンシルステート
	ComPtr<ID3D11DepthStencilView>	m_depthStencilView;			// Ｚバッファ、ステンシルビュー
	ComPtr<ID3D11RasterizerState>	m_rasterState;				// ラスターステータス

	D3D_DRIVER_TYPE					m_DriverType;				// ドライバタイプ
	D3D_FEATURE_LEVEL				m_FeatureLevel;				// 機能レベル

	ComPtr<ID3D11BlendState>		m_alphaEnableBlendingState;		// アルファブレンドステート（有効）
	ComPtr<ID3D11BlendState>		m_alphaDisableBlendingState;	// アルファブレンドステート（無効）

	ComPtr<ID3D11SamplerState>		m_samplerstate;					// サンプラーステート

	int								m_Width = 0;				// バックバッファＸサイズ
	int								m_Height = 0;				// バックバッファＹサイズ
public:

	CDirectXGraphics(const CDirectXGraphics&) = delete;
	CDirectXGraphics& operator=(const CDirectXGraphics&) = delete;
	CDirectXGraphics(CDirectXGraphics&&) = delete;
	CDirectXGraphics& operator=(CDirectXGraphics&&) = delete;

	~CDirectXGraphics() {
		Exit();
	}

	static CDirectXGraphics* GetInstance() {
		static CDirectXGraphics instance;
		return &instance;
	}

	// 初期処理
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height, bool fullscreen);

	// 終了処理
	void Exit();

	// デバイスGET
	ID3D11Device*	GetDXDevice() const {
		return m_lpDevice.Get();
	}

	// イミディエイトデバイスGET
	ID3D11DeviceContext* GetImmediateContext() const {
		return m_lpImmediateContext.Get();
	}

	// SWAPチェインGET
	IDXGISwapChain* GetSwapChain()const {
		return m_lpSwapChain.Get();
	}

	// レンダリングターゲットGET
	ID3D11RenderTargetView* GetRenderTargetView() const {
		return m_lpRenderTargetView.Get();
	}

	// depthstencil view
	ID3D11DepthStencilView* GetDepthStencilView() const {
		return m_depthStencilView.Get();
	}

	// ビューポートの高さを取得する
	int GetViewPortHeight() const {
		return m_Height;
	}

	// ビューポートの幅を取得する
	int GetViewPortWidth() const {
		return m_Width;
	}

	// アルファブレンド有効化
	void TurnOnAlphaBlending();

	// アルファブレンド無効化
	void TurnOffAlphaBlending();

	// Zバッファ無効化
	void TurnOffZbuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = m_lpRenderTargetView.Get();

		m_lpImmediateContext->OMSetRenderTargets(
			1,										// ターゲット
			rtvtable,								// ビューテーブル
			nullptr									// 深度バッファなし
		);
	}

	// Zバッファ有効化
	void TurnOnZBuffer() {
		ID3D11RenderTargetView*  rtvtable[1];

		rtvtable[0] = m_lpRenderTargetView.Get();

		m_lpImmediateContext->OMSetRenderTargets(
			1,										// ターゲット
			rtvtable,								// ビューテーブル
			m_depthStencilView.Get()				// 深度バッファなし
		);
	}
};
