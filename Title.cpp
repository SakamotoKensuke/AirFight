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
//#include "Cpngtexture.h"
#include "Title.h"
#include "Sprite.h"

Sprite g_spraite;
//Cpngtexture g_texture;

void TitleInit() {

	////DX11初期化
	//DX11Init(
	//	Application::Instance()->GetHWnd(),
	//	Application::CLIENT_WIDTH,
	//	Application::CLIENT_HEIGHT, false);

	////DIRECTINPUT初期化
	//CDirectInput::GetInstance().Init(
	//	Application::Instance()->GetHInst(),
	//	Application::Instance()->GetHWnd(),
	//	Application::CLIENT_WIDTH,
	//	Application::CLIENT_HEIGHT);

	g_spraite.Create("assets/Title.png");

	g_spraite.SetPosition(0,0,0);
	g_spraite.SetScale(1280, 720, 0);
	/*g_spraite.SetColor({ 1, 1, 1, 1 });*/
		

	CDirectInput::GetInstance().GetKeyBuffer();

	//g_texture.Init("assets/ta.png");
}
void TitleInput() {
	CDirectInput::GetInstance().GetKeyBuffer();

}

// タイトル描画
void TitleRender() {

	float col[4] = { 0,0,1,1 };	//{ R , G , B , A }

//描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	//プロジェクション変換行列
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	//ビュー変換行列
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);	
	
	DX11MtxIdentity(mtx);
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	//ビュー変換行列
	DX11MtxIdentity(mtx);
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	//g_texture.Draw();
	TurnOffZbuffer();
	g_spraite.Draw();
	TurnOnZbuffer();

	//描画後処理
	DX11AfterRender();
}

//　タイトル更新
void TitleUpdate() {
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
			now_scene = SCENE::GAME;
			printf("game");
		}
}

void TitleDispose() {

	//DX11AfterRender();

	DX11Uninit();

}