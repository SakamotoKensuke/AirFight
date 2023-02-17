#include "map.h"
#include "dx11mathutil.h"


bool Map::Init() {
	// 行列初期化
	DX11MtxIdentity(m_mtx);

	// モデルの読み込みm
	bool sts = m_map.Init(
		"assets/skydome/skydome.x",	// ファイル名 
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/pstexcol.hlsl",			// ピクセルシェーダー
		"assets/skydome/");

	return sts;
}

void Map::Draw() {
	// モデル描画
	m_map.Draw(m_mtx);

}

void Map::Update()
{

}

void Map::Finalize()
{
	m_map.Uninit();
}
