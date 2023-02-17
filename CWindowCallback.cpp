//*****************************************************************************
//!	@file	CWindowCallback.cpp
//!	@brief	
//!	@note	ウインドウコールバッククラス
//!	@author	T.Suzuki
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include "CWindowCallback.h"

//==============================================================================
//!	@fn		OnKeyUp
//!	@brief	キー押下
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnKeyUp( HWND   h_wnd,
								    UINT   h_Message,
									WPARAM h_WParam,
									LPARAM h_LParam )
{
	return DefWindowProc(h_wnd, h_Message, h_WParam, h_LParam );
}


//==============================================================================
//!	@fn		OnKeyDown
//!	@brief	キー押下
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnKeyDown( HWND   h_wnd, 
									  UINT   h_Message,
									  WPARAM h_WParam,
									  LPARAM h_LParam )
{
	switch (h_WParam) {
	case VK_ESCAPE:
		DestroyWindow(h_wnd);
	default:
		return DefWindowProc(h_wnd, h_Message, h_WParam, h_LParam);
	}
	return 0;
}

//==============================================================================
//!	@fn		OnSysKeyUp
//!	@brief	システムキー解放
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnSysKeyUp( HWND   h_wnd,
									   UINT   h_Message,
									   WPARAM h_WParam,
									   LPARAM h_LParam )
{	
	return DefWindowProc(h_wnd, h_Message, h_WParam, h_LParam );
}


//==============================================================================
//!	@fn		OnSysKeyDown
//!	@brief	システムキー押下
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnSysKeyDown( HWND   h_wnd,
									     UINT   h_Message,
									     WPARAM h_WParam,
									     LPARAM h_LParam )
{
	return DefWindowProc(h_wnd, h_Message, h_WParam, h_LParam );
}

//==============================================================================
//!	@fn		OnClose
//!	@brief	Ｘ
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnClose( HWND   h_wnd,
									UINT   h_Message,
									WPARAM h_WParam,
									LPARAM h_LParam )
{
	PostQuitMessage( 0 );
    return 0;
}

//==============================================================================
//!	@fn		OnDestroy
//!	@brief	終了
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnDestroy( HWND   h_wnd,
									  UINT   h_Message,
									  WPARAM h_WParam,
									  LPARAM h_LParam )
{
	PostQuitMessage( 0 );
	return 0;
}


//==============================================================================
//!	@fn		OnPaint
//!	@brief	再描画
//!	@param	
//!	@retval	
//==============================================================================
LRESULT CWindowCallback :: OnPaint( HWND   h_wnd,
									UINT   h_Message,
									WPARAM h_WParam,
									LPARAM h_LParam )
{  
	return DefWindowProc(h_wnd, h_Message, h_WParam, h_LParam );
}

//******************************************************************************
//	End of file.
//******************************************************************************
