#pragma once
#include <string>
#include <assimp/cimport.h>

class AssimpScene {
private:
	const aiScene* m_Scene = nullptr;			// assimp scene
public:
	// 初期処理
	bool Init(std::string filename);
	// 終了処理
	void Exit();

	// シーン取得
	const aiScene* GetScene() const;

	// アニメーションを持っているか
	bool HasAnimation();
};