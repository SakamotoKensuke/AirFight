#include	"player.h"
#include	"drawaxis.h"

bool Player::Init() {
	// 行列初期化
	DX11MtxIdentity(m_mtx);

	return true;
}

void Player::Draw() {

	// モデル描画
	m_model->Draw(m_mtx);

}

void Player::DrawWithAxis() {

	// モデル描画
	drawaxis(m_mtx, 200, m_pos);
	Draw();

}

void Player::Update() {

	XMFLOAT3 angle = { 0,0,0 };

	bool keyinput = false;

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{
		angle.x += 1.0f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		angle.x -= 1.0f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		angle.y -= 1.0f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		angle.y += 1.0f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F1))
	{
		angle.z += 1.0f;
		keyinput = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F2))
	{
		angle.z -= 1.0f;
		keyinput = true;
	}

	XMFLOAT4 axisX;
	XMFLOAT4 axisY;
	XMFLOAT4 axisZ;

	//X軸を取り出す
	axisX.x = m_mtx._11;
	axisX.y = m_mtx._12;
	axisX.z = m_mtx._13;
	axisX.w = 0.0f;

	//y軸を取り出す
	axisY.x = m_mtx._21;
	axisY.y = m_mtx._22;
	axisY.z = m_mtx._23;
	axisY.w = 0.0f;

	//z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	if (keyinput)
	{
		XMFLOAT4 qt;

		//行列からクオータニオンを生成
		DX11GetQtfromMatrix(m_mtx, qt);

		XMFLOAT4 qtx, qty, qtz;

		//指定軸回転のクオータニオンを生成
		DX11QtRotationAxis(qtx, axisX, angle.x);
		DX11QtRotationAxis(qty, axisY, angle.y);
		DX11QtRotationAxis(qtz, axisZ, angle.z);

		//クオータニオンを合成
		XMFLOAT4 tempqt1;
		DX11QtMul(tempqt1, qt, qtx);

		XMFLOAT4 tempqt2;
		DX11QtMul(tempqt2, qty, qtz);

		XMFLOAT4 tempqt3;
		DX11QtMul(tempqt3, tempqt1, tempqt2);

		//クオータニオンをノーマライズ
		DX11QtNormalize(tempqt3, tempqt3);

		//クオータニオンから行列を作成
		DX11MtxFromQt(m_mtx, tempqt3);

	}

	//Z軸を取り出す
	axisZ.x = m_mtx._31;
	axisZ.y = m_mtx._32;
	axisZ.z = m_mtx._33;
	axisZ.w = 0.0f;

	m_pos.x += axisZ.x * 1.0f;
	m_pos.y += axisZ.y * 1.0f;
	m_pos.z += axisZ.z * 1.0f;

	m_mtx._41 = m_pos.x;
	m_mtx._42 = m_pos.y;
	m_mtx._43 = m_pos.z;
}

void Player::Finalize() 
{
	m_model->Uninit();
}
