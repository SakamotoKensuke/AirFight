//*****************************************************************************
//!	@file	CWindow.cpp
//!	@brief	
//!	@note	ウインドウ関連クラス
//!	@author	T.Suzuki
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include "CWindow.h"

#include "CWindowCallback.h"

#include "macro.h"

//==============================================================================
//!	@fn		WindowProc
//!	@brief	ウインドウプロシージャ
//!	@param	ウインドウハンドル
//!	@param	メッセージ
//!	@param	Wパラメータ
//!	@param	Lパラメータ
//!	@retval	終了状況
//==============================================================================
static LRESULT APIENTRY WindowProc( HWND   h_Handle,
								    UINT   h_Message,
									WPARAM h_WParam,
									LPARAM h_LParam )
{

	// コールバックが設定されていなければ終了
	CWindowCallback* callback = CWindow::Instance()->GetCallback();
	if( callback == nullptr )
		return DefWindowProc( h_Handle, h_Message, h_WParam, h_LParam );

	// メッセージ処理
	switch( h_Message )
	{
		// キー押下
		case WM_KEYDOWN:
			return callback->OnKeyDown( h_Handle, h_Message, h_WParam, h_LParam );

		// キー解放
		case WM_KEYUP:
			return callback->OnKeyUp( h_Handle, h_Message, h_WParam, h_LParam );

		// システムキー押下
		case WM_SYSKEYDOWN:
			return callback->OnSysKeyDown( h_Handle, h_Message, h_WParam, h_LParam );

		// システムキー解放
		case WM_SYSKEYUP:
			return callback->OnSysKeyUp( h_Handle, h_Message, h_WParam, h_LParam );

		// ×
		case WM_CLOSE:
			return callback->OnClose( h_Handle, h_Message, h_WParam, h_LParam );

		// ウインドウ解放
		case WM_DESTROY:
			return callback->OnDestroy( h_Handle, h_Message, h_WParam, h_LParam );

		// 再描画
		case WM_PAINT:
			return callback->OnPaint( h_Handle, h_Message, h_WParam, h_LParam );

		default:
			return DefWindowProc( h_Handle, h_Message, h_WParam, h_LParam );
	}

	return 0;	
}

//==============================================================================
//!	@fn		CWindow
//!	@brief	コンストラクタ
//!	@param	
//!	@retval	
//==============================================================================
CWindow :: CWindow()
{}

//==============================================================================
//!	@fn		~CWindow
//!	@brief	デストラクタ
//!	@param	
//!	@retval	
//==============================================================================
CWindow::~CWindow()
{
	SAFE_DELETE( m_cpCallback );
}

//==============================================================================
//!	@fn		Instance
//!	@brief	インスタンス取得
//!	@param	
//!	@retval	インスタンス
//==============================================================================
CWindow* CWindow::Instance()
{
	static CWindow Instance;
	return &Instance;
}

//==============================================================================
//!	@fn		RegisterClass
//!	@brief	ウインドウクラス登録
//!	@param	インスタンスハンドル
//!	@param	クラスネーム
//!	@param	ウインドウスタイル
//!	@retval true 成功 / false 失敗
//==============================================================================
bool CWindow :: RegisterClass( HINSTANCE	 h_Instance,
							   const char*	 h_ClassName,
							   unsigned long h_Style )
{
    // ウィンドウクラス登録
	WNDCLASSEX WndClassEx;
    WndClassEx.cbSize        = sizeof( WNDCLASSEX );
    WndClassEx.style         = h_Style;
    WndClassEx.lpfnWndProc   = WindowProc;
    WndClassEx.cbClsExtra    = 0L;
    WndClassEx.cbWndExtra    = 0L;
    WndClassEx.hInstance     = h_Instance;
    WndClassEx.hIcon         = nullptr;
    WndClassEx.hCursor       = nullptr;
    WndClassEx.hbrBackground = nullptr;
    WndClassEx.lpszMenuName  = nullptr;
    WndClassEx.lpszClassName = h_ClassName;
    WndClassEx.hIconSm       = NULL;

    if( !RegisterClassEx( &WndClassEx ) )
	{
    	MessageBox( NULL, "RegisterClassEx", "Error!", MB_OK );
        return false;
    }

    return true;
}

//==============================================================================
//!	@fn		CreateWindow
//!	@brief	ウインドウ作成
//!	@param	インスタンスハンドル
//!	@param	ウインドウスタイル
//!	@param	ウインドウスタイル
//!	@param	親ハンドル
//!	@param	幅サイズ
//!	@param	高さサイズ
//!	@param	クラスネーム
//!	@param	ウインドウタイトル
//!	@param	コールバック関数
//!	@retval ウインドウポインタ
//==============================================================================
void CWindow :: SetWindow( HINSTANCE		h_Instance,
						   unsigned long	h_Style,
						   unsigned long	h_ExStyle,
						   HWND				h_Parent,
						   long				h_Width,
						   long				h_Height,
						   const char*		h_ClassName,
						   const char*		h_Title,
						   bool				FULLSCREEN)		
{
	if (FULLSCREEN) {
		m_Handle = CreateWindowEx(
			h_ExStyle,
			h_ClassName,					// ウィンドウクラスの名前
			h_Title,						// タイトル
			h_Style,						// ウィンドウスタイル
			0, 0,							// ウィンドウ位置 縦, 横
			h_Width, h_Height,				// ウィンドウサイズ
			NULL,							// 親ウィンドウなし
			(HMENU)NULL,					// メニューなし
			h_Instance,						// インスタンスハンドル
			(LPVOID)NULL);					// 追加引数なし
	}
	else {
		RECT	rWindow, rClient;

		m_Handle = CreateWindowEx(
			h_ExStyle,
			h_ClassName,					// ウィンドウクラスの名前
			h_Title,						// タイトル
			WS_CAPTION | WS_SYSMENU,		// ウィンドウスタイル
			0, 0,							// ウィンドウ位置 縦, 横(あとで中央に移動させます)
			h_Width, h_Height,				// ウィンドウサイズ
			HWND_DESKTOP,					// 親ウィンドウなし
			(HMENU)NULL,					// メニューなし
			h_Instance,						// インスタンスハンドル
			(LPVOID)NULL);					// 追加引数なし

		// ウインドウサイズを再計算（Metricsだけでは、フレームデザインでクライアント領域サイズが変わってしまうので）
		GetWindowRect(m_Handle, &rWindow);
		GetClientRect(m_Handle, &rClient);
		int width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + h_Width;
		int height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + h_Height;
		SetWindowPos(
			m_Handle,
			NULL,
			GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2,
			GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
			width - 1,
			height - 1,
			SWP_NOZORDER);
	}

	if (!m_Handle) {
		MessageBox(nullptr,"CreateWindow Error","error",MB_OK);
	}
	
	// コールバック関数設定
    m_cpCallback = new CWindowCallback;
}

//==============================================================================
//!	@fn		ExecMessage
//!	@brief	メッセージ処理
//!	@param	
//!	@retval	true 正常 / false 異常
//==============================================================================
bool CWindow :: ExecMessage()
{
	// メッセージ確認
//    if( PeekMessage( &m_Message, NULL, 0, 0, PM_NOREMOVE ) )
	while (PeekMessage(&m_Message, NULL, 0, 0, PM_NOREMOVE))
	{
		// メッセージなし
		if( !( ::GetMessage( &m_Message, NULL, 0, 0 ) ) ) 
			return false;

        TranslateMessage( &m_Message );	// キーメッセージの変換
        DispatchMessage( &m_Message );	// メッセージの送出
    }

    return true;
}

//==============================================================================
//!	@fn		GetMessage
//!	@brief	ウインドウメッセージを取得
//!	@param	
//!	@retval	ウインドウメッセージ
//==============================================================================
long CWindow :: GetMessage() const
{
    return m_Message.message;
}

//==============================================================================
//!	@fn		GetHandle
//!	@brief	ウィンドウハンドル取得
//!	@param	
//!	@retval	ウィンドウハンドル
//==============================================================================
HWND CWindow :: GetHandle() const
{
    return m_Handle;
}

//==============================================================================
//!	@fn		GetCallback
//!	@brief	コールバック取得
//!	@param	
//!	@retval	コールバック
//==============================================================================
CWindowCallback* CWindow :: GetCallback() const
{
    return m_cpCallback;
}

//******************************************************************************
//	End of file.
//******************************************************************************