#pragma once
#include <string>
#include <assimp/cimport.h>

class AssimpScene {
private:
	const aiScene* m_Scene = nullptr;			// assimp scene
public:
	// ��������
	bool Init(std::string filename);
	// �I������
	void Exit();

	// �V�[���擾
	const aiScene* GetScene() const;

	// �A�j���[�V�����������Ă��邩
	bool HasAnimation();
};