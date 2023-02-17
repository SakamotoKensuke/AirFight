#include	<DirectXMath.h>
#include	<vector>
#include	"line.h"

using namespace DirectX;

Line g_line;

void drawaxis(XMFLOAT4X4 mtx,float length,XMFLOAT3 pos) {
	// ê¸èâä˙âª
	std::vector<Line::MyVertex> v;
	v.resize(2);

	static bool first = true;

	if (first) {
		// èâä˙âª
		v[0].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
		v[1].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);

		v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	
		g_line.Init(v);

		first = false;
	}

	// Xé≤
	v[0].vertex.x = pos.x + mtx._11*-length / 2.0f;
	v[0].vertex.y = pos.y + mtx._12*-length / 2.0f;
	v[0].vertex.z = pos.z + mtx._13*-length / 2.0f;

	v[1].vertex.x = pos.x + mtx._11*length / 2.0f;
	v[1].vertex.y = pos.y + mtx._12*length / 2.0f;
	v[1].vertex.z = pos.z + mtx._13*length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	g_line.SetVertex(v);
	g_line.Draw();


	// Yé≤
	v[0].vertex.x = pos.x + mtx._21*-length / 2.0f;
	v[0].vertex.y = pos.y + mtx._22*-length / 2.0f;
	v[0].vertex.z = pos.z + mtx._23*-length / 2.0f;

	v[1].vertex.x = pos.x + mtx._21*length / 2.0f;
	v[1].vertex.y = pos.y + mtx._22*length / 2.0f;
	v[1].vertex.z = pos.z + mtx._23*length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	g_line.SetVertex(v);
	g_line.Draw();

	// Zé≤
	v[0].vertex.x = pos.x + mtx._31*-length / 2.0f;
	v[0].vertex.y = pos.y + mtx._32*-length / 2.0f;
	v[0].vertex.z = pos.z + mtx._33*-length / 2.0f;

	v[1].vertex.x = pos.x + mtx._31*length / 2.0f;
	v[1].vertex.y = pos.y + mtx._32*length / 2.0f;
	v[1].vertex.z = pos.z + mtx._33*length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	g_line.SetVertex(v);
	g_line.Draw();

}