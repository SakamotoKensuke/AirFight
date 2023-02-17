//*****************************************************************************
//!	@file	Application.h
//!	@brief	
//!	@note	アプリケーションクラス
//!	@author	T.Suzuki
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<Windows.h>
#include	<stdio.h>
#include	<cinttypes>

//==============================================================================
//!	@class	CApplication
//!	@brief	アプリケーション(シングルトン)
//==============================================================================
class Application
{
public:
	static const char*			WINDOW_TITLE;					// = "アプリケーションクラス";
	static const char*			WINDOW_CLASS_NAME;				// = "win32app";

	static const uint32_t		WINDOW_STYLE_WINDOWED;			// = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
	static const uint32_t		WINDOW_EX_STYLE_WINDOWED;		// = (0);
	static const uint32_t		WINDOW_STYLE_FULL_SCREEN;		// = (WS_VISIBLE | WS_POPUP);
	static const uint32_t		WINDOW_EX_STYLE_FULL_SCREEN;	// = (0);

	static const uint32_t		CLIENT_WIDTH;					// = 1280;
	static const uint32_t		CLIENT_HEIGHT;					// = 720;

	static uint32_t				SYSTEM_WIDTH;					// = 0;
	static uint32_t				SYSTEM_HEIGHT;					// = 0;

	static const float			FPS;							// = 60;

private:
	HWND					m_hWnd;							// Windowハンドル
	HINSTANCE				m_hInst;						// インスタンスハンドル
	uint32_t				m_SystemCounter;				// システムカウンタ
	FILE*					m_fp;							// デバッグ用コンソール

private:
	Application();											// コンストラクタ
	
	Application( const Application& );					// コピー
	Application& operator = (const Application&) {}		// =
	
	void Input(uint64_t deltataime);					// 入力
	void Update(uint64_t deltataime);					// 更新
	void Render(uint64_t deltataime);					// 描画

public:
	virtual ~Application();								// デストラクタ
	static void InitSystemWH();							// システム幅高さ初期化

	bool Init( HINSTANCE h_cpInstance );				// システム有効化
	void Dispose();										// システム無効化
	unsigned long MainLoop();								// メインループ

	// メンバ取得関数
	static Application* Instance();						// インスタンス取得
	HWND			 	GetHWnd();							// ウィンドウハンドル
	HINSTANCE			GetHInst();						// インスタンスハンドル
};

//******************************************************************************
//	End of file.
//******************************************************************************
