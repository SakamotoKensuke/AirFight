//*****************************************************************************
//!	@file	CWindow.h
//!	@brief	
//!	@note	ウインドウ関連クラス
//!	@author	
//*****************************************************************************
#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include	<Windows.h>

class CWindowCallback;

//=============================================================================
//!	@class	CWindow
//!	@brief	ウインドウ関連クラス(シングルトン)
//=============================================================================
class CWindow
{
	HWND				m_Handle;
	MSG					m_Message;
	CWindowCallback*	m_cpCallback;

public:
	static CWindow* Instance();

	bool ExecMessage();
	bool RegisterClass( HINSTANCE	  hInstance,
						const char*	  h_ClassName,
						unsigned long h_Style );
	void SetWindow(HINSTANCE		h_Instance,
		unsigned long	h_Style,
		unsigned long	h_ExStyle,
		HWND			h_Parent,
		long			h_Width,
		long			h_Height,
		const char*		h_ClassName,
		const char*		h_Title,
		bool			FULLSCREEN);

	long GetMessage() const;
	HWND GetHandle() const;
	CWindowCallback* GetCallback() const;

private:
	CWindow();
	virtual ~CWindow();
	CWindow( const CWindow& );
	CWindow& operator = (const CWindow&) {}
};


//******************************************************************************
//	End of file.
//******************************************************************************

