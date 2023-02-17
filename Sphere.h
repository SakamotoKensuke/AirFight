#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include	<wrl/client.h>
#include	"Shader.h"

constexpr float PI = DirectX::XM_PI;

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// インデックスバッファ
	ComPtr<ID3D11VertexShader> m_pVertexShader;	// 頂点シェーダー
	ComPtr<ID3D11PixelShader> m_pPixelShader;	// ピクセルシェーダー
	ComPtr<ID3D11InputLayout>  m_pVertexLayout;	// 頂点レイアウト
	unsigned int		m_divX;					// 水平方向の分割数
	unsigned int		m_divY;					// 垂直方向の分割数
	float				m_radius;				// 半径
	XMFLOAT4			m_color;				// 頂点カラー

	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// インデックスデータ
	struct Face{
		unsigned int idx[3];
	};

	std::vector<Vertex>		m_vertex;			//　頂点データ
	std::vector<Face>		m_face;				//	インデックスデータ

	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
	void CreateVertex();
	// 正規化
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
public:
	// 描画
	void Draw(ID3D11DeviceContext* device);

	// 初期化
	bool Init(float r,						// 半径
		int division_horizontal,			// 水平方向の分割数
		int division_vertical,				// 垂直方向の分割数
		XMFLOAT4	color,					// 頂点カラー
		ID3D11Device* device);
};