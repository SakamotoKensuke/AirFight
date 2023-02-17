#include	<math.h>
#include	"quat.h"

namespace GM31 {
	// 長さを求める
	float length(Vector3 v) {
		float l;
		l = sqrtf(v.x * v.x + v.y *v.y + v.z*v.z);
		return l;
	}

	// 正規化
	Vector3 normalize(Vector3 v) {

		float l = length(v);
		Vector3 t;
		t.x = v.x / l;
		t.y = v.y / l;
		t.z = v.z / l;

		return t;
	}

	// 外積計算
	Vector3 crossvector(Vector3 p1, Vector3 p2) {

		Vector3 ans;

		ans.x = p1.y*p2.z - p1.z*p2.y;
		ans.y = p1.z*p2.x - p1.x*p2.z;
		ans.z = p1.x*p2.y - p1.y*p2.x;

		return ans;
	}

	// 内積計算
	float dotvector(Vector3 p1, Vector3 p2) {

		float ans;
		p1 = normalize(p1);
		p2 = normalize(p2);

		ans = p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;

		return ans;
	}

	Quaternion MulQt(Quaternion qt1, Quaternion qt2) {

		/*
			q⊗p=(qwpx−qzpy+qypz+qxpw,
				 qzpx+qwpy−qxpz+qypw,
				−qypx+qxpy+qwpz+qzpw,
				−qxpx−qypy−qzpz+qwpw)

		*/
		Quaternion ans;

		ans.x = qt1.w  *qt2.x - qt1.z *qt2.y + qt1.y *qt2.z + qt1.x *qt2.w;
		ans.y = qt1.z  *qt2.x + qt1.w *qt2.y - qt1.x *qt2.z + qt1.y *qt2.w;
		ans.z = -qt1.y  *qt2.x + qt1.x *qt2.y + qt1.w *qt2.z + qt1.z *qt2.w;
		ans.w = -qt1.x  *qt2.x - qt1.y *qt2.y - qt1.z *qt2.z + qt1.w *qt2.w;

		return ans;
	}

	Quaternion MulQtVec(Quaternion q, Vector3 v) {
		// 共役クオータニオン
		Quaternion conjugateqt;
		Quaternion qt;

		conjugateqt.x = -q.x;
		conjugateqt.y = -q.y;
		conjugateqt.z = -q.z;
		conjugateqt.w = q.w;

		qt.x = v.x;
		qt.y = v.y;
		qt.z = v.z;
		qt.w = 0.0f;

		Quaternion ansqt;

		ansqt = MulQt(q, qt);

		ansqt = MulQt(ansqt, conjugateqt);

		return ansqt;
	}

	Quaternion looakat(Vector3 forward,		// 前方ベクトル
		Vector3 srcp,			// 現在位置
		Vector3 destp) {		// 目的位置
		Vector3 temp;
		float dot;

		// 目的の方向
		temp.x = destp.x - srcp.x;
		temp.y = destp.y - srcp.y;
		temp.z = destp.z - srcp.z;

		Quaternion ansqt;

		ansqt = RotationArc(forward, temp, dot);

		return ansqt;
	}

	Quaternion RotationArc(Vector3 v0, Vector3 v1, float& d) {

		Vector3		Axis;		// 軸
		Quaternion	q;			// クオータニオン

		// 正規化
		v0 = normalize(v0);
		v1 = normalize(v1);

		// 外積で軸を求める
		Axis = crossvector(v0, v1);

		// 内積を求める
		d = dotvector(v0, v1);

		// ターゲットの方向と自機の方向がほとんど一致した場合、内積の値が１を超える場合がある（浮動小数点の誤差のため）
		if (d > 1.0) d = 1.0;							// 補正してやる
		// ターゲットの方向と自機の方向がほとんど一致した場合、内積の値が-１を下回る場合がある（浮動小数点の誤差のため）
		if (d <= -1.0) d = -1.0f;						// 補正してやる

		float s = (float)sqrtf((1 + d) * 2);
		if (s == 0.0f) {

			q.x = 0.0f;
			q.y = 0.0f;
			q.z = 0.0f;
			q.w = 1.0f;
		}
		else {
			q.x = Axis.x / s;
			q.y = Axis.y / s;
			q.z = Axis.z / s;
			q.w = s / 2;
		}

		return q;
	}
};