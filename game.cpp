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
	// DX11初期化
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// カメラが必要
	DirectX::XMFLOAT3 eye(0, 00, -30);		// カメラの位置
	DirectX::XMFLOAT3 lookat(0, 0, 0);		// 注視点
	DirectX::XMFLOAT3 up(0, 1, 0);			// カメラの上向きベクトル

	CCamera::GetInstance()->Init(
		10.0f,							// ニアクリップ
		10000.0f,						// ファークリップ
		XM_PI / 5.0f,					// 視野角
		static_cast<float>(Application::CLIENT_WIDTH),		// スクリーン幅
		static_cast<float>(Application::CLIENT_HEIGHT),		// スクリーンの高さ
		eye, lookat, up);					// カメラのデータ

	// 平行光源をセット
	DX11LightInit(
		DirectX::XMFLOAT4(1, 1, -1, 0));

	// DIRECTINPUT初期化
	CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,			// スクリーン幅
		Application::CLIENT_HEIGHT			// スクリーンの高さ
		);


	//ステージ初期化
	m_map.Init();

	//タイトル初期化
	TitleInit();

	// アルファブレンド有効化
	TurnOnAlphablend();

	// 空中戦初期化
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
			// 空中戦更新
			AirFightUpdate();

			// ライトの位置を修正
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

// ゲーム画面描画
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

			// 描画前処理
			DX11BeforeRender(col);

			XMFLOAT4X4 mtx;

			// プロジェクション変換行列取得
			mtx = CCamera::GetInstance()->GetProjectionMatrix();
			DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

			// ビュー変換行列を取得
			mtx = CCamera::GetInstance()->GetCameraMatrix();
			DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

			//モデル描画
			//m_map.Draw();

			// 空中戦描画
			AirFightDraw();

			// 描画後処理
			DX11AfterRender();
			break;
		}
		case SCENE::END:
		{
			EndRender();
			break;
		}
	}

	//描画後処理
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