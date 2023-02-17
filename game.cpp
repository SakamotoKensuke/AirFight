#include <cinttypes>
#include "dx11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "Application.h"
#include "CDirectInput.h"
#include "AirFight.h"
#include "map.h"
#include "game.h"
#include "Title.h"
#include "end.h"

Map m_map;

void GameInit() {
	// DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// �J�������K�v
	DirectX::XMFLOAT3 eye(0, 00, -30);		// �J�����̈ʒu
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// �����_
	DirectX::XMFLOAT3 up(0, 1, 0);			// �J�����̏�����x�N�g��

	CCamera::GetInstance()->Init(
		10.0f,							// �j�A�N���b�v
		10000.0f,						// �t�@�[�N���b�v
		XM_PI / 5.0f,					// ����p
		static_cast<float>(Application::CLIENT_WIDTH),		// �X�N���[����
		static_cast<float>(Application::CLIENT_HEIGHT),		// �X�N���[���̍���
		eye, lookat, up);					// �J�����̃f�[�^

	// ���s�������Z�b�g
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// DIRECTINPUT������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// �X�N���[����
		Application::CLIENT_HEIGHT			// �X�N���[���̍���
		);


	//�X�e�[�W������
	m_map.Init();

	//�^�C�g��������
	TitleInit();

	// �A���t�@�u�����h�L����
	TurnOnAlphablend();

	// �󒆐평����
	AirFightInit();
}

void GameInput(uint64_t dt) {
	CDirectInput::GetInstance().GetKeyBuffer();
}

void GameUpdate(uint64_t dt) {


	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		//XA_Play((SOUND_LABEL)(SOUND_LABEL_SE001));

		now_scene = SCENE::GAME;

		//	printf("game");
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_G)) {
		//XA_Play((SOUND_LABEL)(SOUND_LABEL_SE001));

		now_scene = SCENE::END;

		//	printf("game");
	}

	switch (now_scene)
	{
		case SCENE::TITLE:
		{
			TitleUpdate();
			break;
		}
		case SCENE::GAME:
		{
			// �󒆐�X�V
			AirFightUpdate();

			// ���C�g�̈ʒu���C��
			DirectX::XMFLOAT3 eye = CCamera::GetInstance()->GetEye();
			DX11LightUpdate(DirectX::XMFLOAT4(eye.x, eye.y, eye.z, 0));
			break;
		}
		case SCENE::END:
		{
			EndUpdate();
			break;
		}
	}
}

// �Q�[����ʕ`��
void GameRender(uint64_t dt) {
	switch (now_scene)
	{
		case SCENE::TITLE:
		{
			TitleRender();
			break;
		}
		case SCENE::GAME:
		{
			float col[4] = { 1,0,0,1 };

			// �`��O����
			DX11BeforeRender(col);

			XMFLOAT4X4 mtx;

			// �v���W�F�N�V�����ϊ��s��擾
			mtx = CCamera::GetInstance()->GetProjectionMatrix();
			DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

			// �r���[�ϊ��s����擾
			mtx = CCamera::GetInstance()->GetCameraMatrix();
			DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

			//���f���`��
			//m_map.Draw();

			// �󒆐�`��
			AirFightDraw();

			// �`��㏈��
			DX11AfterRender();
			break;
		}
		case SCENE::END:
		{
			EndRender();
			break;
		}
	}

	//�`��㏈��
	DX11AfterRender();
}

void GameDispose()
{
	switch (now_scene)
	{
		case SCENE::TITLE:
		{
			TitleDispose();
			break;
		}
		case SCENE::GAME:
		{
			AirFightExit();

			DX11Uninit();
			break;
		}
		case SCENE::END:
		{
			EndDispose();
			break;
		}
	}

}