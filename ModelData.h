#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <assimp/scene.h>
#include <map>
#include <assimp/Importer.hpp>
#include "Mesh.h"
#include "CDirectxGraphics.h"
#include "assimpscene.h"
#include "vertexproto.h"

class ModelData
{
public:
	ModelData();
	~ModelData();

	// �R�s�[�֎~
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;

	// ���[�u�֎~
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	bool Load(std::string resourcefolder, std::string filename);
	
	void Draw(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4& mtxworld);

	void Exit();

	const std::vector<Mesh>& GetMeshes()const {
		return m_meshes;
	}

	void LoadMaterial();						// �}�e���A����ǂݍ���
private:
	AssimpScene m_assimpscene;					// assimp scene
	std::vector<Mesh> m_meshes;					// ���b�V���̏W�������f��
	std::string m_directory;					// �e�N�X�`���t�@�C���̈ʒu
	std::vector<Texture> m_texturesloaded;		// ���Ƀ��[�h����Ă���e�N�X�`��
	std::vector<Material> m_materials;			// �}�e���A���Q

	void processNode(aiNode* node, const aiScene* scene);				// �m�[�h�����
	Mesh processMesh(aiMesh* mesh, const aiScene* scene,int meshidx);	// ���b�V�������
																																				// �}�e���A���ɑΉ������e�N�X�`�����擾����
	std::vector<Texture> loadMaterialTextures(
		aiMaterial * mat, 
		aiTextureType type, 
		std::string typeName, 
		const aiScene * scene);
};
