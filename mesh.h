#pragma once
#include <string>
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>
#include <wrl/client.h>
#include "vertexproto.h"

// テクスチャ構造体
struct Texture {
	std::string type;							// テクスチャ種類
	std::string path;							// テクスチャパス名
	ID3D11Resource*	texres;						// テクスチャリソース
	ID3D11ShaderResourceView *texture;			// SRV
};

using Microsoft::WRL::ComPtr;

class Mesh {
public:
	ID3D11ShaderResourceView*    m_whitesrv;	// 真っ白ＳＲＶ
	ID3D11Resource*				m_whiteres;	// 真っ白RES

	std::vector<Vertex> m_vertices;			// 頂点データ
	std::vector<unsigned int> m_indices;	// インデックス
	std::vector<Texture> m_textures;		// テクスチャ
	ID3D11Device* m_dev;					// デバイス	
	Material m_mtrl;						// マテリアル

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures,Material mtrl);

	void Draw(ID3D11DeviceContext *devcon);
	void Close();
private:
	ID3D11Buffer*  m_VertexBuffer = nullptr;	// 頂点バッファ
	ID3D11Buffer*  m_IndexBuffer = nullptr;		// インデックスバッファ
	ID3D11Buffer*  m_cbmtrl = nullptr;			// マテリアル用コンスタントバッファ

	bool setupMesh();
};