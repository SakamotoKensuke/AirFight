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

Player	g_player;		// �v���C���I�u�W�F�N�g
Skydome g_skydome;		//�X�J�C�h�[��
CBillBoard	g_billboard; //�r���{�[�h
std::array<Enemy,3>	g_enemy;		//�G


//�e
std::vector<std::unique_ptr<Bullet>>	g_bullets;
//�z�[�~���O
std::vector<std::unique_ptr<HomingMissile>>	g_homingBullets;

BoundingSphere	g_bs;		//�����pBS
BoundingSphere	g_bsenemy;	//�G�pBS
BoundingSphere	g_bshomming; //�z�[�~���O�pBS

//�󒆐�Ŏg�p���郂�f��
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

//�e�̕`��
void DrawBullets()
{
	//�S�e�`��
	for (auto& b : g_bullets)
	{
		b->Drow();
	}
}

//�e�X�V
void UpdateBullets()
{
	//�S�e�X�V
	for (auto& b : g_bullets)
	{
		b->Update();
	}

	//����ł�e����
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
	//�e�̏�����
	p->Init();		//����������
	p->SetModel(
		ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::MISSILE)].modelname)
	);
	//�����ʒu�Z�b�g
	p->SetInitialPos(XMFLOAT3(mtx._41,mtx._42,mtx._43));
	//���˕����Z�b�g
	p->SetDirection(mtx);
	//�Ō���ɋʂ�ǉ�
				//vector�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	g_bullets.emplace_back(std::move(p));
}

//�z�[�~���O�̕`��
void DrawHomingBullets()
{
	for (auto& h : g_homingBullets)
	{
		h->Draw();
	}
}

//�z�[�~���O�X�V
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

	//����ł�z�[�~���O����
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
	//std::cout << "���W" << std::endl;
}

//�z�[�~���O����
void FireHomis(XMFLOAT4X4 mtx) {
	std::unique_ptr<HomingMissile> p;

	p = std::make_unique<HomingMissile>();
	//�e�̏�����
	p->Init();
	p->SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::MISSILE)].modelname));
	p->SetInitailQuaternion(g_player.GetMtx());
	//p->SetVelocity(XMFLOAT3(0.0f, 0.0f, 2.0f)); //Velocity = ���x
	
	//�����ʒu�Z�b�g
	p->SetInitialPos(mtx._41, mtx._42, mtx._43);

	//���Ă�G�I��
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
	//���˕������Z�b�g
	/*p->SetDirection(mtx);*/
	//�Ō���ɋ���ǉ�
	//�x�N�^�[�R���e�i�Ƀ��j�[�N�|�C���^���Z�b�g
	g_homingBullets.emplace_back(std::move(p));
}
 
//FPS�J����
void FPSCamera()
{
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;

	//�����_�v�Z
	XMFLOAT3 lookat;
	lookat.x = mtx._41 + zaxis.x;
	lookat.y = mtx._42 + zaxis.y;
	lookat.z = mtx._43 + zaxis.z;

	//�J�����ʒu�v�Z
	XMFLOAT3 eye;
	eye.x = mtx._41;
	eye.y = mtx._42;
	eye.z = mtx._43;

	//�J�����ʒu�v�Z
	XMFLOAT3 up;
	up.x = mtx._21;
	up.y = mtx._22;
	up.z = mtx._23;

	//�J�����N���X�ɃZ�b�g
	CCamera::GetInstance()->SetLookat(lookat);
	CCamera::GetInstance()->SetEye(eye);
	CCamera::GetInstance()->SetUp(up);

	CCamera::GetInstance()->CreateCameraMatrix();

}

//�Œ蒍���_�J����
void FixedCamera()
{
	XMFLOAT3 lookat;

	lookat = g_player.GetPos();

	CCamera::GetInstance()->SetLookat(lookat);

	CCamera::GetInstance()->CreateCameraMatrix();
}

void DelayCamera()
{
	//�v���C���[�̍s��擾
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();			//��s�@�̍s��擾

	//�r���[�ϊ��s���ύX
	static int svidx = 0;				//�ۑ��ʒu�������C���f�b�N�X�l
	static bool firstflag = false;		//100�܂ŕۑ��������Ă��邩�������t���O

	static std::array<XMFLOAT3, 100> zaxis;	//z��
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;

	static std::array<XMFLOAT3, 100> yaxis;	//y��
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;

	static std::array<XMFLOAT3, 100> pos;	//���݈ʒu
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;

	svidx++;
	if (svidx > zaxis.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}

	//30�t���[���O�̏����g�p���ăJ�������쐬
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

	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	up = yaxis[useidx];

	//�r���[�ϊ��s�񐶐�
	CCamera::GetInstance()->SetLookat(lookat);	//�����_�Z�b�g
	CCamera::GetInstance()->SetEye(eye);		//�J�����̈ʒu�Z�b�g
	CCamera::GetInstance()->SetUp(up);			//�J�����̏�����x�N�g���Z�b�g

	CCamera::GetInstance()->CreateCameraMatrix();			//�r���[�ϊ��s��쐬

}

void TPSCamera()
{
	//�s��擾
	XMFLOAT4X4 mtx;
	mtx = g_player.GetMtx();

	//Z���擾
	XMFLOAT3 zaxis;
	zaxis.x = mtx._31;
	zaxis.y = mtx._32;
	zaxis.z = mtx._33;


	XMFLOAT3 yaxis;	//y��
	yaxis.x = mtx._21;
	yaxis.y = mtx._22;
	yaxis.z = mtx._23;

	XMFLOAT3 pos;	//���݈ʒu
	pos.x = mtx._41;
	pos.y = mtx._42;
	pos.z = mtx._43;



	//30�t���[���O�̏����g�p���ăJ�������쐬

	XMFLOAT3 eye;		//���_
	XMFLOAT3 lookat;	//�����_
	XMFLOAT3 up;		//������x�N�g��

	lookat = pos;
	lookat.x = pos.x;
	lookat.y = pos.y;
	lookat.z = pos.z;

	eye.x = pos.x - 30 * zaxis.x;
	eye.y = pos.y - 30 * zaxis.y;
	eye.z = pos.z - 30 * zaxis.z;

	up = yaxis;

	//�r���[�ϊ��s�񐶐�
	CCamera::GetInstance()->SetLookat(lookat);	//�����_�Z�b�g
	CCamera::GetInstance()->SetEye(eye);		//�J�����̈ʒu�Z�b�g
	CCamera::GetInstance()->SetUp(up);			//�J�����̏�����x�N�g���Z�b�g

	CCamera::GetInstance()->CreateCameraMatrix();			//�r���[�ϊ��s��쐬

}

void AirFightInit() {

	//�󒆐�Ŏg�p���郂�f�������ׂēǂݍ���
	for (int i = 0; i < g_modellist.size(); i++)
	{
		ModelMgr::GetInstance().LoadModel(
			g_modellist[i].modelname,
			g_modellist[i].vsfilename,
			g_modellist[i].psfilename,
			g_modellist[i].texfolder);
	}

	// �v���C��������
	g_player.Init();
	g_player.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::PLAYER)].modelname));

	//�X�J�C�h�[��������
	g_skydome.Init();
	g_skydome.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::SKYDOME)].modelname));
	
	//�G������
	for (auto& e : g_enemy) {
		e.Init();
		e.SetModel(ModelMgr::GetInstance().GetModelPtr(g_modellist[static_cast<int>(MODELID::ENEMY)].modelname));
	}




	//���E������
	{
		//���f���̑S���_�𔲂��o��
		const CModel* pmodel = g_player.GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		//���E��������
		g_bs.Init(
			vertices,					//���_�Q
			XMFLOAT4(1, 1, 1, 0.3f));	//���_�J���[
	}

	//���f���̑S���_�𔲂��o��
	{
		//���f���̑S���_�𔲂��o��
		const CModel* pmodel = g_enemy[0].GetModel();
		const ModelData& md = pmodel->GetModelData();
		const std::vector<Mesh>& meshes = md.GetMeshes();

		std::vector<XMFLOAT3> vertices;

		for (auto& m : meshes) {
			for (auto& v : m.m_vertices) {
				vertices.emplace_back(v.m_Pos);
			}
		}
		//���E��������
		g_bsenemy.Init(
			vertices,					//���_�Q
			XMFLOAT4(1, 1, 1, 0.3));	//���_�J���[
	}


	//�r���{�[�h
	g_billboard.Init(
		0.0f, 0.0f, 0.0f,		//�ʒu 
		10, 10,					//���A����
		XMFLOAT4(1, 0, 0, 1)	//���_�J���[
	);

	XMFLOAT2 uv[4]{
		{6.0f / 8.0f, 0.0f},
		{7.0f / 8.0f, 0.0f},
		{6.0f / 8.0f, 1.0f / 4.0f},
		{7.0f / 8.0f, 1.0f / 4.0f},
	};

	//�e�N�X�`�����W�𒸓_�o�b�t�@�փZ�b�g
	g_billboard.SetUV(uv);

	//�e�N�X�`���ǂݍ���
	g_billboard.LoadTexTure(
		"assets/UI64x64.png");
}

void AirFightUpdate() {

	// �v���C���X�V
	g_player.Update();
	g_bs.UpdatePosition(g_player.GetMtx());


	TPSCamera();

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F3))
	{
		DelayCamera();
		//FPSCamera();
	}

	//�J�����؂�ւ�
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F4))
	{
		//DelayCamera();
		FPSCamera();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE))
	{
		FireBullet(g_player.GetMtx());
	}

	//�e�X�V
	UpdateBullets();

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_H))
	{
		FireHomis(g_player.GetMtx());
	}
	//�z�[�~���O�e�X�V
	UpdateHoming();


	//�G�X�V
	for (auto& e : g_enemy) {
		e.Update();
		e.AutoPilot();
		g_bsenemy.UpdatePosition(e.GetMtx());
	}

}

void AirFightDraw() {

	// �v���C���`��
	g_player.Draw();

	//�X�J�C�h�[���`��
	g_skydome.Draw();

	//�G�`��
	for (auto& e : g_enemy) {
		e.DrawWithAxis();
	}

	// �v���C���`��(���[�J�����W���`��t��)
	g_player.DrawWithAxis();


	//�e�`��
	DrawBullets();

	//�z�[�~���O�`��
	DrawHomingBullets();


	// ���E���`��
	g_bs.Draw();

	//// �r���{�[�h�`��
	////�ʒu���Z�b�g�ix, y, z�j
	//g_billboard.SetPosition(0, 0, 0);

	//g_billboard.DrawBillBoard(
	//	//�r���[�ϊ��s��
	//	CCamera::GetInstance()->GetCameraMatrix());


}

void AirFightExit() {

	// �v���C���I������
	g_player.Finalize();

	//�G�I������
	for (auto& e : g_enemy) {
		e.Finalize();
	}

	//�X�J�C�h�[���I������
	g_skydome.Finalize();

	//���E���I������
	g_bs.Exit();

	//���E���I������
	g_bsenemy.Exit();

	//�r���{�[�h�I������
	g_billboard.Dispose();
}