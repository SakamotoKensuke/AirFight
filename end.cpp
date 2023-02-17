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

	//DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT, false);

	//DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);


	//カメラが必要

	//g_stage.Init();
	//g_stage.Draw();

	CDirectInput::GetInstance().GetKeyBuffer();

	g_texture2.Init("assets/ta.png");
}
void EndInput() {
	CDirectInput::GetInstance().GetKeyBuffer();

}

// タイトル描画
void EndRender() {



	float col[4] = { 0,0,0,1 };	//{ R , G , B , A }


//描画前処理
	DX11BeforeRender(col);

	XMFLOAT4X4 mtx;

	//プロジェクション変換行列
	mtx = CCamera::GetInstance()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	//ビュー変換行列
	mtx = CCamera::GetInstance()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	//モデル描画
	//g_model.Draw(g_mtx);

	//マップ描画
	//g_stage.Draw();

	//g_texture.Draw();

	//描画後処理
	DX11AfterRender();
}

//　タイトル更新
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