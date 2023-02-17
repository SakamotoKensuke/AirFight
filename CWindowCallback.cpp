//*****************************************************************************
//!	@file	CWindowCallback.cpp
//!	@brief	
//!	@note	�E�C���h�E�R�[���o�b�N�N���X
//!	@author	T.Suzuki
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include "CWindowCallback.h"

//==============================================================================
//!	@fn		OnKeyUp
//!	@brief	�L�[����
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
//!	@brief	�L�[����
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
//!	@brief	�V�X�e���L�[���
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
//!	@brief	�V�X�e���L�[����
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
//!	@brief	�w
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
//!	@brief	�I��
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
//!	@brief	�ĕ`��
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
