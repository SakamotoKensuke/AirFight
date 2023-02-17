//*****************************************************************************
//!	@file	Application.cpp
//!	@brief	
//!	@note	�A�v���P�[�V�����N���X
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <Crtdbg.h>
#include <Windows.h>
#include <chrono>
#include <thread>
//#include <cinttypes>
#include "Application.h"
#include "CWindow.h"
#include "macro.h"
#include "game.h"

//-----------------------------------------------------------------------------
// �X�^�e�B�b�N�@�����o�[
//-----------------------------------------------------------------------------
const char* Application::WINDOW_TITLE = "�󒆐�@��s�@�\��";
const char* Application::WINDOW_CLASS_NAME = "win32app";

const uint32_t		Application::WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
const uint32_t		Application::WINDOW_EX_STYLE_WINDOWED = (0);
const uint32_t		Application::WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
const uint32_t		Application::WINDOW_EX_STYLE_FULL_SCREEN = (0);

const uint32_t		Application::CLIENT_WIDTH = 1280;
const uint32_t		Application::CLIENT_HEIGHT = 720;

uint32_t			Application::SYSTEM_WIDTH = 0;
uint32_t			Application::SYSTEM_HEIGHT = 0;

const float			Application::FPS = 60;

//==============================================================================
//!	@fn		CApplication
//!	@brief	�R���X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
Application :: Application(): m_SystemCounter		( 0 )
{}

//==============================================================================
//!	@fn		~Application
//!	@brief	�f�X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
Application :: ~Application()
{
	Dispose();
}

//==============================================================================
//!	@fn		GetInstance
//!	@brief	�C���X�^���X�擾
//!	@param	
//!	@retval	�C���X�^���X
//==============================================================================
Application* Application::Instance()
{
	static Application Instance;

	return &Instance;
}

//==============================================================================
//!	@fn		InitSystemWH
//!	@brief	�V�X�e���̈�̕��ƍ�����������
//!	@param	
//!	@retval	
//==============================================================================
void Application :: InitSystemWH()
{

}

//==============================================================================
//!	@fn		Init
//!	@brief	������
//!	@param	�C���X�^���X�n���h��
//!	@retval	
//==============================================================================
bool Application :: Init( HINSTANCE h_Instance )
{
	// �������[���[�N�����o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);

	// �R���\�[�������蓖�Ă�
	AllocConsole();

	// �W���o�͂̊��蓖��
	freopen_s(&m_fp, "CON", "w", stdout);    

	// ���ƍ���������
	InitSystemWH();

	// �E�C���h�E�쐬
	CWindow* window = CWindow::Instance();
	window->RegisterClass( h_Instance, WINDOW_CLASS_NAME, CS_OWNDC );

	window->SetWindow( h_Instance,
					   WINDOW_STYLE_WINDOWED,
					   WINDOW_EX_STYLE_WINDOWED,
					   NULL,
					   CLIENT_WIDTH + SYSTEM_WIDTH,
					   CLIENT_HEIGHT + SYSTEM_HEIGHT,
					   WINDOW_CLASS_NAME,
					   WINDOW_TITLE,
					   false);
			
	// HWND
	m_hWnd = window->GetHandle();

	//
	m_hInst = h_Instance;
	return true;
}

//==============================================================================
//!	@fn		Dispose
//!	@brief  �I������
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Dispose()
{
	// �W���o�̓N���[�Y
	fclose(m_fp);
	// �R���\�[���J��
	::FreeConsole();

	return;
}

//==============================================================================
//!	@fn		MainLoop
//!	@brief	���C�����[�v
//!	@param	
//!	@retval	���b�Z�[�WID
//==============================================================================
unsigned long Application :: MainLoop()
{
	MSG		msg;	
	ZeroMemory( &msg, sizeof( msg ) );	

	CWindow* window = CWindow::Instance();

	uint64_t current_time = 0;
	uint64_t last_time = 0;

	// �Q�[���̏�������
	GameInit();

	// �^�C�}�𑜓x���~���b��
	::timeBeginPeriod(1);

	while (window->ExecMessage()) {

		// timeGetTime�֐��́A�~���b�P�ʂŃV�X�e���������擾���܂��B 
		// �V�X�e�����Ԃ́AWindows���N�����Ă���̌o�ߎ��Ԃł��B
		current_time = ::timeGetTime();	

		uint64_t delta_time = current_time - last_time;

		last_time = current_time;

		GameInput(delta_time);		// �ްѓ���	
		GameUpdate(delta_time);		// �ްэX�V
		GameRender(delta_time);		// �ްѕ`��

		int64_t sleep_time = 16666 - delta_time;

		if (sleep_time > 0) {
			float tt = sleep_time / 1000.0f;
//			printf("sleep:%f \n", tt);
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//�w�肵�����Ύ��Ԃ������X���b�h���u���b�N���� (function template)
		}
	}

	// �^�C�}�𑜓x�����ɖ߂�
	::timeEndPeriod(1);

	// �Q�[���̏I������
	GameDispose();

	return window->GetMessage();
}

//==============================================================================
//!	@fn		Input
//!	@brief	�L�[����
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Input(uint64_t deltataime)
{
}

//==============================================================================
//!	@fn		Update
//!	@brief	�X�V
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Update(uint64_t deltataime)
{
	// �V�X�e���J�E���^
	m_SystemCounter ++;
}

//==============================================================================
//!	@fn		Render
//!	@brief	�`��
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Render(uint64_t deltatime)
{
}

//==============================================================================
//!	@fn		GetHWnd
//!	@brief	HWND �擾
//!	@param	
//!	@retval	HWND
//==============================================================================
HWND Application :: GetHWnd()
{
	return m_hWnd;
}

//==============================================================================
//!	@fn		GetHInst
//!	@brief	HINSTANCE �擾
//!	@param	
//!	@retval	HINSTANCE
//==============================================================================
HINSTANCE Application::GetHInst()
{
	return m_hInst;
}

//******************************************************************************
//	End of file.
//******************************************************************************
