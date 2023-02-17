//*****************************************************************************
//!	@file	Application.h
//!	@brief	
//!	@note	�A�v���P�[�V�����N���X
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
//!	@brief	�A�v���P�[�V����(�V���O���g��)
//==============================================================================
class Application
{
public:
	static const char*			WINDOW_TITLE;					// = "�A�v���P�[�V�����N���X";
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
	HWND					m_hWnd;							// Window�n���h��
	HINSTANCE				m_hInst;						// �C���X�^���X�n���h��
	uint32_t				m_SystemCounter;				// �V�X�e���J�E���^
	FILE*					m_fp;							// �f�o�b�O�p�R���\�[��

private:
	Application();											// �R���X�g���N�^
	
	Application( const Application& );					// �R�s�[
	Application& operator = (const Application&) {}		// =
	
	void Input(uint64_t deltataime);					// ����
	void Update(uint64_t deltataime);					// �X�V
	void Render(uint64_t deltataime);					// �`��

public:
	virtual ~Application();								// �f�X�g���N�^
	static void InitSystemWH();							// �V�X�e��������������

	bool Init( HINSTANCE h_cpInstance );				// �V�X�e���L����
	void Dispose();										// �V�X�e��������
	unsigned long MainLoop();								// ���C�����[�v

	// �����o�擾�֐�
	static Application* Instance();						// �C���X�^���X�擾
	HWND			 	GetHWnd();							// �E�B���h�E�n���h��
	HINSTANCE			GetHInst();						// �C���X�^���X�n���h��
};

//******************************************************************************
//	End of file.
//******************************************************************************
