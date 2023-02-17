#pragma once
#include	<d3d11.h>

namespace GM31 {
	struct Quaternion {
		float x;
		float y;
		float z;
		float w;
	};

	struct Vector3 {
		float x, y, z;
	};

	float length(Vector3 v);
	Vector3 normalize(Vector3 v);
	Vector3 crossvector(Vector3 p1, Vector3 p2);
	float dotvector(Vector3 p1, Vector3 p2);
	Quaternion looakat(Vector3 forward,		// 前方ベクトル
		Vector3 srcp,			// 現在位置
		Vector3 destp);

	Quaternion MulQtVec(Quaternion q, Vector3 v);
	Quaternion RotationArc(Vector3 v0, Vector3 v1, float& d);
	Quaternion MulQt(Quaternion qt1, Quaternion qt2);
}
