//*****************************************************************************
//!	@file	CWindow.cpp
//!	@brief	
//!	@note	�E�C���h�E�֘A�N���X
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
//!	@brief	�E�C���h�E�v���V�[�W��
//!	@param	�E�C���h�E�n���h��
//!	@param	���b�Z�[�W
//!	@param	W�p�����[�^
//!	@param	L�p�����[�^
//!	@retval	�I����
//==============================================================================
static LRESULT APIENTRY WindowProc( HWND   h_Handle,
								    UINT   h_Message,
									WPARAM h_WParam,
									LPARAM h_LParam )
{

	// �R�[���o�b�N���ݒ肳��Ă��Ȃ���ΏI��
	CWindowCallback* callback = CWindow::Instance()->GetCallback();
	if( callback == nullptr )
		return DefWindowProc( h_Handle, h_Message, h_WParam, h_LParam );

	// ���b�Z�[�W����
	switch( h_Message )
	{
		// �L�[����
		case WM_KEYDOWN:
			return callback->OnKeyDown( h_Handle, h_Message, h_WParam, h_LParam );

		// �L�[���
		case WM_KEYUP:
			return callback->OnKeyUp( h_Handle, h_Message, h_WParam, h_LParam );

		// �V�X�e���L�[����
		case WM_SYSKEYDOWN:
			return callback->OnSysKeyDown( h_Handle, h_Message, h_WParam, h_LParam );

		// �V�X�e���L�[���
		case WM_SYSKEYUP:
			return callback->OnSysKeyUp( h_Handle, h_Message, h_WParam, h_LParam );

		// �~
		case WM_CLOSE:
			return callback->OnClose( h_Handle, h_Message, h_WParam, h_LParam );

		// �E�C���h�E���
		case WM_DESTROY:
			return callback->OnDestroy( h_Handle, h_Message, h_WParam, h_LParam );

		// �ĕ`��
		case WM_PAINT:
			return callback->OnPaint( h_Handle, h_Message, h_WParam, h_LParam );

		default:
			return DefWindowProc( h_Handle, h_Message, h_WParam, h_LParam );
	}

	return 0;	
}

//==============================================================================
//!	@fn		CWindow
//!	@brief	�R���X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
CWindow :: CWindow()
{}

//==============================================================================
//!	@fn		~CWindow
//!	@brief	�f�X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
CWindow::~CWindow()
{
	SAFE_DELETE( m_cpCallback );
}

//==============================================================================
//!	@fn		Instance
//!	@brief	�C���X�^���X�擾
//!	@param	
//!	@retval	�C���X�^���X
//==============================================================================
CWindow* CWindow::Instance()
{
	static CWindow Instance;
	return &Instance;
}

//==============================================================================
//!	@fn		RegisterClass
//!	@brief	�E�C���h�E�N���X�o�^
//!	@param	�C���X�^���X�n���h��
//!	@param	�N���X�l�[��
//!	@param	�E�C���h�E�X�^�C��
//!	@retval true ���� / false ���s
//==============================================================================
bool CWindow :: RegisterClass( HINSTANCE	 h_Instance,
							   const char*	 h_ClassName,
							   unsigned long h_Style )
{
    // �E�B���h�E�N���X�o�^
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
//!	@brief	�E�C���h�E�쐬
//!	@param	�C���X�^���X�n���h��
//!	@param	�E�C���h�E�X�^�C��
//!	@param	�E�C���h�E�X�^�C��
//!	@param	�e�n���h��
//!	@param	���T�C�Y
//!	@param	�����T�C�Y
//!	@param	�N���X�l�[��
//!	@param	�E�C���h�E�^�C�g��
//!	@param	�R�[���o�b�N�֐�
//!	@retval �E�C���h�E�|�C���^
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
			h_ClassName,					// �E�B���h�E�N���X�̖��O
			h_Title,						// �^�C�g��
			h_Style,						// �E�B���h�E�X�^�C��
			0, 0,							// �E�B���h�E�ʒu �c, ��
			h_Width, h_Height,				// �E�B���h�E�T�C�Y
			NULL,							// �e�E�B���h�E�Ȃ�
			(HMENU)NULL,					// ���j���[�Ȃ�
			h_Instance,						// �C���X�^���X�n���h��
			(LPVOID)NULL);					// �ǉ������Ȃ�
	}
	else {
		RECT	rWindow, rClient;

		m_Handle = CreateWindowEx(
			h_ExStyle,
			h_ClassName,					// �E�B���h�E�N���X�̖��O
			h_Title,						// �^�C�g��
			WS_CAPTION | WS_SYSMENU,		// �E�B���h�E�X�^�C��
			0, 0,							// �E�B���h�E�ʒu �c, ��(���ƂŒ����Ɉړ������܂�)
			h_Width, h_Height,				// �E�B���h�E�T�C�Y
			HWND_DESKTOP,					// �e�E�B���h�E�Ȃ�
			(HMENU)NULL,					// ���j���[�Ȃ�
			h_Instance,						// �C���X�^���X�n���h��
			(LPVOID)NULL);					// �ǉ������Ȃ�

		// �E�C���h�E�T�C�Y���Čv�Z�iMetrics�����ł́A�t���[���f�U�C���ŃN���C�A���g�̈�T�C�Y���ς���Ă��܂��̂Łj
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
	
	// �R�[���o�b�N�֐��ݒ�
    m_cpCallback = new CWindowCallback;
}

//==============================================================================
//!	@fn		ExecMessage
//!	@brief	���b�Z�[�W����
//!	@param	
//!	@retval	true ���� / false �ُ�
//==============================================================================
bool CWindow :: ExecMessage()
{
	// ���b�Z�[�W�m�F
//    if( PeekMessage( &m_Message, NULL, 0, 0, PM_NOREMOVE ) )
	while (PeekMessage(&m_Message, NULL, 0, 0, PM_NOREMOVE))
	{
		// ���b�Z�[�W�Ȃ�
		if( !( ::GetMessage( &m_Message, NULL, 0, 0 ) ) ) 
			return false;

        TranslateMessage( &m_Message );	// �L�[���b�Z�[�W�̕ϊ�
        DispatchMessage( &m_Message );	// ���b�Z�[�W�̑��o
    }

    return true;
}

//==============================================================================
//!	@fn		GetMessage
//!	@brief	�E�C���h�E���b�Z�[�W���擾
//!	@param	
//!	@retval	�E�C���h�E���b�Z�[�W
//==============================================================================
long CWindow :: GetMessage() const
{
    return m_Message.message;
}

//==============================================================================
//!	@fn		GetHandle
//!	@brief	�E�B���h�E�n���h���擾
//!	@param	
//!	@retval	�E�B���h�E�n���h��
//==============================================================================
HWND CWindow :: GetHandle() const
{
    return m_Handle;
}

//==============================================================================
//!	@fn		GetCallback
//!	@brief	�R�[���o�b�N�擾
//!	@param	
//!	@retval	�R�[���o�b�N
//==============================================================================
CWindowCallback* CWindow :: GetCallback() const
{
    return m_cpCallback;
}

//******************************************************************************
//	End of file.
//******************************************************************************