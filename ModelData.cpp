#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <assimp/cimport.h>
#include <unordered_map>
#include <vector>
#include "Shader.h"
#include "memory.h"
#include "ModelData.h"
#include "vertexproto.h"
#include "DX11Settransform.h"
#include "CDirectxGraphics.h"
#include "utftosjisconv.h"

std::vector<Texture> ModelData::loadMaterialTextures(
	aiMaterial* mtrl, 
	aiTextureType type, 
	std::string typeName, 
	const aiScene * scene)
{
	std::vector<Texture> textures;		// このマテリアルに関連づいたDIFFUSEテクスチャのリスト
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// マテリアルからテクスチャ個数を取得し(基本は1個)ループする
	for (unsigned int i = 0; i < mtrl->GetTextureCount(type); i++)
	{
		aiString str;

		// マテリアルからｉ番目のテクスチャファイル名を取得する
		mtrl->GetTexture(type, i, &str);

		// もし既にロードされたテクスチャであればロードをスキップする
		bool skip = false;

		// ロード済みテクスチャ数分ループする
		for (unsigned int j = 0; j < m_texturesloaded.size(); j++)
		{
			// ファイル名が同じであったら読み込まない
			if (std::strcmp(m_texturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// 読み込み済みのテクスチャ情報をDIFFUSEテクスチャのリストにセット
				textures.push_back(m_texturesloaded[j]);
				skip = true; 
				break;
			}
		}
		if (!skip)
		{   // まだ読み込まれていなかった場合
			Texture tex;

			std::string filename = std::string(str.C_Str());
			std::string filenameonly = ExtractFileName(filename, "\\/");		// ファイル名を取得
			filename = m_directory + filenameonly;				// リソースディレクトリ＋ファイル名
//			MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.texres,&tex.texture);
			if (!sts) {
				// ｆｉｌｅ名がＵＴＦ８で設定されていた場合に対応
				filenameonly = utf8_to_multi_winapi(std::string(filenameonly.c_str()));
				filename = m_directory + filenameonly;	// リソースディレクトリ＋ファイル名

				bool sts = CreateSRVfromFile(
					filename.c_str(),
					dev, devcon, &tex.texres, &tex.texture);
				if (!sts) {
					MessageBox(nullptr, "Texture couldn't be loaded", "Error!", MB_ICONERROR | MB_OK);
					tex.texture = nullptr;
				}
			}

			tex.type = typeName;
			tex.path = str.C_Str();
			// テクスチャ情報をDIFFUSEテクスチャのリストにセット
			textures.push_back(tex);
			this->m_texturesloaded.push_back(tex);	// このモデルに関連づいたテクスチャリストにセット
		}
	}

	return textures;
}

ModelData::ModelData()
{
}

ModelData::~ModelData()
{
	Exit();
}

void ModelData::LoadMaterial() {

	// マテリアルが存在するか？
	if (m_assimpscene.GetScene()->HasMaterials()) {
		// マテリアル数 取得
		int nummaterial = m_assimpscene.GetScene()->mNumMaterials;

		for (int i = 0; i < nummaterial; i++) {
			Material mtrl;

			// i番目のマテリアルを取得
			aiMaterial* mat = m_assimpscene.GetScene()->mMaterials[i];

			aiColor3D colordiffuse(1.f, 1.f, 1.f);
			mat->Get(AI_MATKEY_COLOR_DIFFUSE, colordiffuse);
						mtrl.m_Diffuse.x = colordiffuse.r;
						mtrl.m_Diffuse.y = colordiffuse.g;
						mtrl.m_Diffuse.z = colordiffuse.b;
			mtrl.m_Diffuse.x = 1.0f;
			mtrl.m_Diffuse.y = 1.0f;
			mtrl.m_Diffuse.z = 1.0f;

			aiColor3D colorspecular(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_SPECULAR, colorspecular);
			mtrl.m_Specular.x = colorspecular.r;
			mtrl.m_Specular.y = colorspecular.g;
			mtrl.m_Specular.z = colorspecular.b;

			aiColor3D colorambient(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_AMBIENT, colorambient);
			mtrl.m_Ambient.x = colorambient.r;
			mtrl.m_Ambient.y = colorambient.g;
			mtrl.m_Ambient.z = colorambient.b;

			aiColor3D coloremissive(0.f, 0.f, 0.f);
			mat->Get(AI_MATKEY_COLOR_EMISSIVE, coloremissive);
			mtrl.m_Emissive.x = coloremissive.r;
			mtrl.m_Emissive.y = coloremissive.g;
			mtrl.m_Emissive.z = coloremissive.b;

			float power;
			mat->Get(AI_MATKEY_SHININESS_STRENGTH, power);
			mtrl.m_SpecularPower = power;

			m_materials.push_back(mtrl);
		}
	}
}

bool ModelData::Load(std::string resourcefolder,
	std::string filename)
{
	bool sts = m_assimpscene.Init(filename);
	if (!sts) {
		MessageBox(nullptr, "ModelData load error", "error", MB_OK);
		return false;
	}

	m_directory = resourcefolder;		// このモデルのテクスチャが存在するディレクトリ

	LoadMaterial();						// このモデルで使用されているマテリアルを取得する

	// aiノードを解析する
	processNode(m_assimpscene.GetScene()->mRootNode, m_assimpscene.GetScene());

	return true;
}

void ModelData::Draw(ID3D11DeviceContext * devcon, XMFLOAT4X4& mtxworld )
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		// ワールド変換行列
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// 定数バッファセット処理
		m_meshes[i].Draw(devcon);
	}
}

// メッシュの解析
Mesh ModelData::processMesh(aiMesh * mesh, const aiScene * scene,int meshidx)
{
	std::vector<Vertex> vertices;			// 頂点
	std::vector<unsigned int> indices;		// 面の構成情報
	std::vector<Texture> textures;			// テクスチャ
	Material mtrl;

	// 頂点情報を取得
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.m_Pos.x = mesh->mVertices[i].x;
		vertex.m_Pos.y = mesh->mVertices[i].y;
		vertex.m_Pos.z = mesh->mVertices[i].z;

		// 法線ベクトルが存在するか？
		if (mesh->HasNormals()) {
			vertex.m_Normal.x = mesh->mNormals[i].x;
			vertex.m_Normal.y = mesh->mNormals[i].y;
			vertex.m_Normal.z = mesh->mNormals[i].z;
		}
		else {
			vertex.m_Normal.x = 0.0f;
			vertex.m_Normal.y = 0.0f;
			vertex.m_Normal.z = 0.0f;
		}

		// テクスチャ座標（０番目）が存在するか？
		if (mesh->HasTextureCoords(0)) {
			vertex.m_Tex.x = mesh->mTextureCoords[0][i].x;
			vertex.m_Tex.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// テクスチャ情報を取得する
	if (mesh->mMaterialIndex >= 0)
	{
		// このメッシュのマテリアルインデックス値を取得する
		int	mtrlidx = mesh->mMaterialIndex;

		// このメッシュのマテリアルを取得する
		mtrl = m_materials[mtrlidx];

		// シーンからマテリアルデータを取得する
		aiMaterial* material = scene->mMaterials[mtrlidx];

		// このマテリアルに関連づいたテクスチャを取り出す
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// このメッシュで使用しているテクスチャを保存
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	
	// 面の構成情報を取得
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices, textures, mtrl);
}

void ModelData::Exit()
{
	// テクスチャリソースを解放する
	for (auto tex :m_texturesloaded)
	{
		if (tex.texture != nullptr) {
			tex.texture->Release();
		}
	}

	// メッシュの解放
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Close();
	}

	// assimp scene 解放
	m_assimpscene.Exit();

}

// ノードの解析
void ModelData::processNode(aiNode * node, const aiScene * scene)
{	
	// ノード内のメッシュの数分ループする
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];			// ノードのi番目メッシュのインデックスを取得
		aiMesh* mesh = scene->mMeshes[meshindex];	// シーンからメッシュ本体を取り出す

		m_meshes.push_back(this->processMesh(mesh, scene,meshindex));
	}

	// 子ノードについても解析
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}