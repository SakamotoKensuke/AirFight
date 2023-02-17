#include <cinttypes>	
#include "Application.h"
#include "DX11util.h"
#include "CModel.h"
#include "CCamera.h"
#include "CLight.h"
#include "DX11Settransform.h"
#include "dx11mathutil.h"
#include "CDirectInput.h"
#include "game.h"
#include"Cpngtexture.h"
#include "end.h"

Cpngtexture g_texture2;

void EndInit() {

	//DX11������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT, false);

	//DIRECTINPUT������
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);


	//�J�������K�v

	//g_stage.Init();
	//g_stage.Draw();

	CDirectInput::GetInstance().GetKeyBuffer();

	g_texture2.Init("assets/ta.png");
}
void EndInput() {
	CDirectInput::GetInstance().GetKeyBuffer();

}

// �^�C�g���`��
void EndRender() {



	float col[4] = { 0,0,0,1 };	//{ R , G , B , A }


//�`��O����
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	//�v���W�F�N�V�����ϊ��s��
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	//�r���[�ϊ��s��
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	//���f���`��
	//g_model.Draw(g_mtx);

	//�}�b�v�`��
	//g_stage.Draw();

	//g_texture.Draw();

	//�`��㏈��
	DX11AfterRender();
}

//�@�^�C�g���X�V
void EndUpdate() {
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_G)) {

			now_scene = SCENE::END;
			printf("end");
		}
}

void EndDispose() {

	//DX11AfterRender();

	DX11Uninit();

}