#include	<assimp/cimport.h>
#include	<assimp\postprocess.h>
#include	<assimp/cimport.h>
#include	<assimp\scene.h>

#include	"assimpscene.h"

bool AssimpScene::Init(std::string filename) {

	m_Scene = aiImportFile(filename.c_str(),
		aiProcess_ConvertToLeftHanded |
		aiProcessPreset_TargetRealtime_MaxQuality);

	if (m_Scene == nullptr) {
		return false;
	}

	return true;
}

// 終了処理
void AssimpScene::Exit() {
	aiReleaseImport(m_Scene);
}

// シーン取得
const aiScene* AssimpScene::GetScene() const {
	return m_Scene;
}

// アニメーションを持っているか
bool AssimpScene::HasAnimation() {
	return m_Scene->HasAnimations();
}