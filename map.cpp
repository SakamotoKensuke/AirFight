#include "map.h"
#include "dx11mathutil.h"


bool Map::Init() {
	// �s�񏉊���
	DX11MtxIdentity(m_mtx);

	// ���f���̓ǂݍ���m
	bool sts = m_map.Init(
		"assets/skydome/skydome.x",	// �t�@�C���� 
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/pstexcol.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/skydome/");

	return sts;
}

void Map::Draw() {
	// ���f���`��
	m_map.Draw(m_mtx);

}

void Map::Update()
{

}

void Map::Finalize()
{
	m_map.Uninit();
}
