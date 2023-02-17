//*****************************************************************************
//!	@file	CWindowCallback.h
//!	@brief	
//!	@note	コールバッククラス
//!	@author	T.Suzuki
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<Windows.h>

//=============================================================================
//!	@class	CWindowCallback
//!	@brief	コールバッククラス
//=============================================================================
class CWindowCallback
{
public:
	CWindowCallback(){};
	virtual ~CWindowCallback(){};

	virtual LRESULT OnKeyDown	( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnKeyUp		( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnSysKeyDown( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnSysKeyUp	( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnClose		( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnDestroy	( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
	virtual LRESULT OnPaint		( HWND h_Handle, UINT h_Message, WPARAM h_WParam, LPARAM h_LParam );
 
};


//******************************************************************************
//	End of file.
//******************************************************************************
