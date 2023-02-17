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
	std::vector<Texture> textures;		// ���̃}�e���A���Ɋ֘A�Â���DIFFUSE�e�N�X�`���̃��X�g
	ID3D11Device* dev;
	ID3D11DeviceContext* devcon;

	dev = CDirectXGraphics::GetInstance()->GetDXDevice();
	devcon = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// �}�e���A������e�N�X�`�������擾��(��{��1��)���[�v����
	for (unsigned int i = 0; i < mtrl->GetTextureCount(type); i++)
	{
		aiString str;

		// �}�e���A�����炉�Ԗڂ̃e�N�X�`���t�@�C�������擾����
		mtrl->GetTexture(type, i, &str);

		// �������Ƀ��[�h���ꂽ�e�N�X�`���ł���΃��[�h���X�L�b�v����
		bool skip = false;

		// ���[�h�ς݃e�N�X�`���������[�v����
		for (unsigned int j = 0; j < m_texturesloaded.size(); j++)
		{
			// �t�@�C�����������ł�������ǂݍ��܂Ȃ�
			if (std::strcmp(m_texturesloaded[j].path.c_str(), str.C_Str()) == 0)
			{
				// �ǂݍ��ݍς݂̃e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
				textures.push_back(m_texturesloaded[j]);
				skip = true; 
				break;
			}
		}
		if (!skip)
		{   // �܂��ǂݍ��܂�Ă��Ȃ������ꍇ
			Texture tex;

			std::string filename = std::string(str.C_Str());
			std::string filenameonly = ExtractFileName(filename, "\\/");		// �t�@�C�������擾
			filename = m_directory + filenameonly;				// ���\�[�X�f�B���N�g���{�t�@�C����
//			MessageBox(nullptr, filename.c_str(), "load tex", MB_OK);

			bool sts = CreateSRVfromFile(
				filename.c_str(),
				dev, devcon, &tex.texres,&tex.texture);
			if (!sts) {
				// �������������t�s�e�W�Őݒ肳��Ă����ꍇ�ɑΉ�
				filenameonly = utf8_to_multi_winapi(std::string(filenameonly.c_str()));
				filename = m_directory + filenameonly;	// ���\�[�X�f�B���N�g���{�t�@�C����

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
			// �e�N�X�`������DIFFUSE�e�N�X�`���̃��X�g�ɃZ�b�g
			textures.push_back(tex);
			this->m_texturesloaded.push_back(tex);	// ���̃��f���Ɋ֘A�Â����e�N�X�`�����X�g�ɃZ�b�g
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

	// �}�e���A�������݂��邩�H
	if (m_assimpscene.GetScene()->HasMaterials()) {
		// �}�e���A���� �擾
		int nummaterial = m_assimpscene.GetScene()->mNumMaterials;

		for (int i = 0; i < nummaterial; i++) {
			Material mtrl;

			// i�Ԗڂ̃}�e���A�����擾
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

	m_directory = resourcefolder;		// ���̃��f���̃e�N�X�`�������݂���f�B���N�g��

	LoadMaterial();						// ���̃��f���Ŏg�p����Ă���}�e���A�����擾����

	// ai�m�[�h����͂���
	processNode(m_assimpscene.GetScene()->mRootNode, m_assimpscene.GetScene());

	return true;
}

void ModelData::Draw(ID3D11DeviceContext * devcon, XMFLOAT4X4& mtxworld )
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		// ���[���h�ϊ��s��
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, mtxworld);
		// �萔�o�b�t�@�Z�b�g����
		m_meshes[i].Draw(devcon);
	}
}

// ���b�V���̉��
Mesh ModelData::processMesh(aiMesh * mesh, const aiScene * scene,int meshidx)
{
	std::vector<Vertex> vertices;			// ���_
	std::vector<unsigned int> indices;		// �ʂ̍\�����
	std::vector<Texture> textures;			// �e�N�X�`��
	Material mtrl;

	// ���_�����擾
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.m_Pos.x = mesh->mVertices[i].x;
		vertex.m_Pos.y = mesh->mVertices[i].y;
		vertex.m_Pos.z = mesh->mVertices[i].z;

		// �@���x�N�g�������݂��邩�H
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

		// �e�N�X�`�����W�i�O�Ԗځj�����݂��邩�H
		if (mesh->HasTextureCoords(0)) {
			vertex.m_Tex.x = mesh->mTextureCoords[0][i].x;
			vertex.m_Tex.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// �e�N�X�`�������擾����
	if (mesh->mMaterialIndex >= 0)
	{
		// ���̃��b�V���̃}�e���A���C���f�b�N�X�l���擾����
		int	mtrlidx = mesh->mMaterialIndex;

		// ���̃��b�V���̃}�e���A�����擾����
		mtrl = m_materials[mtrlidx];

		// �V�[������}�e���A���f�[�^���擾����
		aiMaterial* material = scene->mMaterials[mtrlidx];

		// ���̃}�e���A���Ɋ֘A�Â����e�N�X�`�������o��
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);

		// ���̃��b�V���Ŏg�p���Ă���e�N�X�`����ۑ�
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	
	// �ʂ̍\�������擾
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
	// �e�N�X�`�����\�[�X���������
	for (auto tex :m_texturesloaded)
	{
		if (tex.texture != nullptr) {
			tex.texture->Release();
		}
	}

	// ���b�V���̉��
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Close();
	}

	// assimp scene ���
	m_assimpscene.Exit();

}

// �m�[�h�̉��
void ModelData::processNode(aiNode * node, const aiScene * scene)
{	
	// �m�[�h���̃��b�V���̐������[�v����
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int meshindex = node->mMeshes[i];			// �m�[�h��i�Ԗڃ��b�V���̃C���f�b�N�X���擾
		aiMesh* mesh = scene->mMeshes[meshindex];	// �V�[�����烁�b�V���{�̂����o��

		m_meshes.push_back(this->processMesh(mesh, scene,meshindex));
	}

	// �q�m�[�h�ɂ��Ă����
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
	}
}