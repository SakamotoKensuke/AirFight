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

	// コピー禁止
	ModelData(const ModelData&) = delete;
	ModelData& operator=(const ModelData&) = delete;

	// ムーブ禁止
	ModelData(ModelData&&) = delete;
	ModelData& operator=(ModelData&&) = delete;

	bool Load(std::string resourcefolder, std::string filename);
	
	void Draw(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4& mtxworld);

	void Exit();

	const std::vector<Mesh>& GetMeshes()const {
		return m_meshes;
	}

	void LoadMaterial();						// マテリアルを読み込む
private:
	AssimpScene m_assimpscene;					// assimp scene
	std::vector<Mesh> m_meshes;					// メッシュの集合がモデル
	std::string m_directory;					// テクスチャファイルの位置
	std::vector<Texture> m_texturesloaded;		// 既にロードされているテクスチャ
	std::vector<Material> m_materials;			// マテリアル群

	void processNode(aiNode* node, const aiScene* scene);				// ノードを解析
	Mesh processMesh(aiMesh* mesh, const aiScene* scene,int meshidx);	// メッシュを解析
																																				// マテリアルに対応したテクスチャを取得する
	std::vector<Texture> loadMaterialTextures(
		aiMaterial * mat, 
		aiTextureType type, 
		std::string typeName, 
		const aiScene * scene);
};
