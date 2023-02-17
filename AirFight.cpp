#include	<array>
#include	<vector>
#include	<string>
#include	"player.h"
#include	"CCamera.h"
#include	"skydome.h"
#include	"enemy.h"
#include	"BoundingSphere.h"
#include	"Bullet.h"
#include	"ModelMgr.h"
#include	"CBillBoard.h"
#include	"quat.h"
#include    "hommingmissile.h"

#include	<iostream>
#include "AirFight.h"
#define	DebugLog(a)  std::cout<<a<<std::endl;

Player	g_player;		// プレイヤオブジェクト
Skydome g_skydome;		//スカイドーム
CBillBoard	g_billboard; //ビルボード
std::array<Enemy,3>	g_enemy;		//敵


//弾
std::vector<std::unique_ptr<Bullet>>	g_bullets;
//ホーミング
std::vector<std::unique_ptr<HomingMissile>>	g_homingBullets;

BoundingSphere	g_bs;		//自分用BS
BoundingSphere	g_bsenemy;	//敵用BS
BoundingSphere	g_bshomming; //ホーミング用BS

//空中戦で使用するモデル
enum class MODELID {
	PLAYER,
	SKYDOME,
	ENEMY,
	MISSILE,
	HOMING,
};

struct ModelDataList
{
	MODELID		id;
	std::string	modelname;
	std::string vsfilename;
	std::string psfilename;
	std::string texfolder;
};

std::vector<ModelDataList>	g_modellist = {
	{MODELID::PLAYER,"assets/f1/f1.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"},
	{MODELID::SKYDOME,"assets/skydome/skydome.x","shader/vs.hlsl","shader/pstexcol.hlsl","assets/skydome/"},
	{MODELID::ENEMY,"assets/f1/f1.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"},
	{MODELID::MISSILE,"assets/f1/TIEShot.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"},
	{MODELID::HOMING,"assets/f1/TIEShot.x","shader/vs.hlsl","shader/ps.hlsl","assets/f1/"}
};

//弾の描画
void DrawBullets()
{
	//全弾描画
	for (auto& b : g_bullets)
	{
		b->Drow();
	}
}

//弾更新
void UpdateBullets()
{
	//全弾更新
	for (auto& b : g_bullets)
	{
		b->Update();
	}

	//死んでる弾解除
	auto it = g_bullets.begin();
	while (it != g_bullets.end())
	{
		if ((*it)->isLive() == false)
		{
			it = g_bullets.erase(it);
		}
		else ++it;
	}
}

void FireBullet(XMFLOAT4X4 mtx)
{
	std::unique_ptr<Bullet> p;
	p = std::make_unique<Bullet>();
	//弾の初期化
	p->Init();		//初期化処理
	p->SetModel(
		ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::MISSILE)].modelname)
	);
	//初期位置セット
	p->SetInitialPos(XMFLOAT3(mtx._41,mtx._42,mtx._43));
	//発射方向セット
	p->SetDirection(mtx);
	//最後尾に玉を追加
				//vectorコンテナにユニークポインタをセット
	g_bullets.emplace_back(std::move(p));
}

//ホーミングの描画
void DrawHomingBullets()
{
	for (auto& h : g_homingBullets)
	{
		h->Draw();
	}
}

//ホーミング更新
void UpdateHoming()
{
	for (auto& h : g_homingBullets)
	{
		XMFLOAT3 playerpos = g_player.GetPos();
		float mini = -1;
		int targetnum = 0;
		int count = 0;
		for (auto i : g_enemy)
		{
			float len;
			DX11p2pLength(playerpos, *i.GetPos(), len);
			if (mini == -1)
			{
				targetnum = count;
				mini = len;
			}
			if (len < mini)
			{
				targetnum = count;
				mini = len;
			}
			count++;
		}
		h->SetTarget(*g_enemy[targetnum].GetPos());
		h->Update();
		h->EnemyCollision(*g_enemy[targetnum].GetPos());
	}

	//死んでるホーミング解除
	auto ih = g_homingBullets.begin();
	while (ih != g_homingBullets.end())
	{
		if ((*ih)->isLive() == false)
		{
			ih = g_homingBullets.erase(ih);
		}
		else ++ih;
	}

	//DebugLog(g_homingBullets[0]->GetPos().x);
	//DebugLog(g_homingBullets[0]->GetPos().y);
	//DebugLog(g_homingBullets[0]->GetPos().z);
	//std::cout << "座標" << std::endl;
}

//ホーミング発射
void FireHomis(XMFLOAT4X4 mtx) {
	std::unique_ptr<HomingMissile> p;

	p = std::make_unique<HomingMissile>();
	//弾の初期化
	p->Init();
	p->SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::MISSILE)].modelname));
	p->SetInitailQuaternion(g_player.GetMtx());
	//p->SetVelocity(XMFLOAT3(0.0f, 0.0f, 2.0f)); //Velocity = 速度
	
	//初期位置セット
	p->SetInitialPos(mtx._41, mtx._42, mtx._43);

	//あてる敵選ぶ
	XMFLOAT3 playerpos = g_player.GetPos();
	float mini = -1;
	int targetnum = 0;
	int count = 0;
	for ( auto i : g_enemy)
	{
		float len;
		DX11p2pLength(playerpos, *i.GetPos(), len);
		if (mini == -1)
		{
			targetnum = count;
			mini = len;
		}
		if (len<mini)
		{
			targetnum = count;
			mini = len;
		}
		count++;
	}
	p->SetTarget(*g_enemy[targetnum].GetPos());
	p->EnemyCollision(*g_enemy[targetnum].GetPos());
	//発射方向をセット
	/*p->SetDirection(mtx);*/
	//最後尾に球を追加
	//ベクターコンテナにユニークポインタをセット
	g_homingBullets.emplace_back(std::move(p));
}
 
//FPSカメラ
void FPSCamera()
{
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//注視点計算
	XMFLOAT3 lookat;
	lookat.x = mtx._41 + zaxis.x;
	lookat.y = mtx._42 + zaxis.y;
	lookat.z = mtx._43 + zaxis.z;

	//カメラ位置計算
	XMFLOAT3 eye;
	eye.x = mtx._41;
	eye.y = mtx._42;
	eye.z = mtx._43;

	//カメラ位置計算
	XMFLOAT3 up;
	up.x = mtx._21;
	up.y = mtx._22;
	up.z = mtx._23;

	//カメラクラスにセット
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();

}

//固定注視点カメラ
void FixedCamera()
{
	XMFLOAT3 lookat;

	lookat = g_player.GetPos();

	CCamera::GetInstance()->SetLookat(lookat);

	CCamera::GetInstance()->CreateCameraMatrix();
}

void DelayCamera()
{
	//プレイヤーの行列取得
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();			//飛行機の行列取得

	//ビュー変換行列を変更
	static int svidx = 0;				//保存位置を示すインデックス値
	static bool firstflag = false;		//100まで保存完了しているかを示すフラグ

	static std::array<XMFLOAT3, 100> zaxis;	//z軸
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	static std::array<XMFLOAT3, 100> yaxis;	//y軸
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	static std::array<XMFLOAT3, 100> pos;	//現在位置
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}

	//30フレーム前の情報を使用してカメラを作成
	int useidx;
	useidx = svidx - 30;

	if (useidx < 0)
	{
		if (firstflag == false)
		{
			useidx = 0;
		}
		else
		{
			useidx = svidx + zaxis.size() - 30;
		}
	}

	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	up = yaxis[useidx];

	//ビュー変換行列生成
	CCamera::GetInstance()->SetLookat(lookat);	//注視点セット
	CCamera::GetInstance()->SetEye(eye);		//カメラの位置セット
	CCamera::GetInstance()->SetUp(up);			//カメラの上向きベクトルセット

	CCamera::GetInstance()->CreateCameraMatrix();			//ビュー変換行列作成

}

void TPSCamera()
{
	//行列取得
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();

	//Z軸取得
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;


	XMFLOAT3 yaxis;	//y軸
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	XMFLOAT3 pos;	//現在位置
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;



	//30フレーム前の情報を使用してカメラを作成

	XMFLOAT3 eye;		//視点
	XMFLOAT3 lookat;	//注視点
	XMFLOAT3 up;		//上向きベクトル

	lookat = pos;
	lookat.x = pos.x;
	lookat.y = pos.y;
	lookat.z = pos.z;

	eye.x = pos.x - 30 * zaxis.x;
	eye.y = pos.y - 30 * zaxis.y;
	eye.z = pos.z - 30 * zaxis.z;

	up = yaxis;

	//ビュー変換行列生成
	CCamera::GetInstance()->SetLookat(lookat);	//注視点セット
	CCamera::GetInstance()->SetEye(eye);		//カメラの位置セット
	CCamera::GetInstance()->SetUp(up);			//カメラの上向きベクトルセット

	CCamera::GetInstance()->CreateCameraMatrix();			//ビュー変換行列作成

}

void AirFightInit() {

	//空中戦で使用するモデルをすべて読み込む
	for (int i = 0; i < g_modellist.size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(
			g_modellist[i].modelname,
			g_modellist[i].vsfilename,
			g_modellist[i].psfilename,
			g_modellist[i].texfolder);
	}

	// プレイヤ初期化
	g_player.Init();
	g_player.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::PLAYER)].modelname));

	//スカイドーム初期化
	g_skydome.Init();
	g_skydome.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::SKYDOME)].modelname));
	
	//敵初期化
	for (auto& e : g_enemy) {
		e.Init();
		e.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::ENEMY)].modelname));
	}




	//境界球生成
	{
		//モデルの全頂点を抜き出す
		const CModel* pmodel = g_player.GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		//境界球初期化
		g_bs.Init(
			vertices,					//頂点群
			XMFLOAT4(1, 1, 1, 0.3f));	//頂点カラー
	}

	//モデルの全頂点を抜き出す
	{
		//モデルの全頂点を抜き出す
		const CModel* pmodel = g_enemy[0].GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		//境界球初期化
		g_bsenemy.Init(
			vertices,					//頂点群
			XMFLOAT4(1, 1, 1, 0.3));	//頂点カラー
	}


	//ビルボード
	g_billboard.Init(
		0.0f, 0.0f, 0.0f,		//位置 
		10, 10,					//幅、高さ
		XMFLOAT4(1, 0, 0, 1)	//頂点カラー
	);

	XMFLOAT2 uv[4]{
		{6.0f / 8.0f, 0.0f},
		{7.0f / 8.0f, 0.0f},
		{6.0f / 8.0f, 1.0f / 4.0f},
		{7.0f / 8.0f, 1.0f / 4.0f},
	};

	//テクスチャ座標を頂点バッファへセット
	g_billboard.SetUV(uv);

	//テクスチャ読み込み
	g_billboard.LoadTexTure(
		"assets/UI64x64.png");
}

void AirFightUpdate() {

	// プレイヤ更新
	g_player.Update();
	g_bs.UpdatePosition(g_player.GetMtx());


	TPSCamera();

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F3))
	{
		DelayCamera();
		//FPSCamera();
	}

	//カメラ切り替え
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F4))
	{
		//DelayCamera();
		FPSCamera();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE))
	{
		FireBullet(g_player.GetMtx());
	}

	//弾更新
	UpdateBullets();

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_H))
	{
		FireHomis(g_player.GetMtx());
	}
	//ホーミング弾更新
	UpdateHoming();


	//敵更新
	for (auto& e : g_enemy) {
		e.Update();
		e.AutoPilot();
		g_bsenemy.UpdatePosition(e.GetMtx());
	}

}

void AirFightDraw() {

	// プレイヤ描画
	g_player.Draw();

	//スカイドーム描画
	g_skydome.Draw();

	//敵描画
	for (auto& e : g_enemy) {
		e.DrawWithAxis();
	}

	// プレイヤ描画(ローカル座標軸描画付き)
	g_player.DrawWithAxis();


	//弾描画
	DrawBullets();

	//ホーミング描画
	DrawHomingBullets();


	// 境界球描画
	g_bs.Draw();

	//// ビルボード描画
	////位置をセット（x, y, z）
	//g_billboard.SetPosition(0, 0, 0);

	//g_billboard.DrawBillBoard(
	//	//ビュー変換行列
	//	CCamera::GetInstance()->GetCameraMatrix());


}

void AirFightExit() {

	// プレイヤ終了処理
	g_player.Finalize();

	//敵終了処理
	for (auto& e : g_enemy) {
		e.Finalize();
	}

	//スカイドーム終了処理
	g_skydome.Finalize();

	//境界球終了処理
	g_bs.Exit();

	//境界球終了処理
	g_bsenemy.Exit();

	//ビルボード終了処理
	g_billboard.Dispose();
}