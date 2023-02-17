#include "enemy.h"
#include "hommingmissile.h"
#include "drawaxis.h"


bool Enemy::Init()
{
    //行列初期化
    DX11MtxIdentity(m_mtx);

    m_actionKeepTime = 10;

    m_speed = 1.0f;

    //シード値決定用乱数
    std::random_device rand_dev;

    //シード値セット
    m_mt.seed(rand_dev());
    m_mt2.seed(rand_dev());

    return true;
}

void Enemy::Draw()
{
    //モデル描画
    m_model->Draw(m_mtx);
}

void Enemy::DrawWithAxis()
{
    //モデル描画
    drawaxis(m_mtx, 200, m_pos);
    m_model->Draw(m_mtx);
}

void Enemy::Update()
{
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

    XMFLOAT4 qt;

    //行列からクオータニオンを生成
    DX11GetQtfromMatrix(m_mtx, qt);

    XMFLOAT4 qtx, qty, qtz;

    //指定軸回転のクオータニオンを生成
    DX11QtRotationAxis(qtx, axisX, m_angle.x);
    DX11QtRotationAxis(qty, axisY, m_angle.y);
    DX11QtRotationAxis(qtz, axisZ, m_angle.z);

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

    //z軸を取り出す
    axisZ.x = m_mtx._31;
    axisZ.y = m_mtx._32;
    axisZ.z = m_mtx._33;
    axisZ.w = 0.0f;

    m_pos.x += axisZ.x * m_speed;
    m_pos.y += axisZ.y * m_speed;
    m_pos.z += axisZ.z * m_speed;

    m_mtx._41 = m_pos.x;
    m_mtx._42 = m_pos.y;
    m_mtx._43 = m_pos.z;



}

void Enemy::Finalize()
{
    m_model->Uninit();
}

void Enemy::AutoPilot()
{
    //乱数生成
    std::uniform_int_distribution<int32_t> rand4(0, static_cast<int>(Enemy::ACTION::MAX_ACTION) - 1);
    std::uniform_int_distribution<int32_t> rand10(10, 50);
    //回転角をいったんリセット
    m_angle.x = m_angle.y = m_angle.z = 0.0f;

    //アクションにより回転角をセット
    switch (m_action)
    {
    case Enemy::ACTION::ACT_STRAIGHT:   //直進
        m_angle.x = m_angle.y = m_angle.z = 0.0f;
        break;

    case Enemy::ACTION::ACT_LEFT:   //左回転Y
        m_angle.y = 0.1f;
        break;

    case Enemy::ACTION::ACT_RIGHT:   //右回転Y
        m_angle.y = -0.1f;
        break;

    case Enemy::ACTION::ACT_UP:   //上回転X
        m_angle.x = 0.1f;
        break;

    case Enemy::ACTION::ACT_DOWN:   //下回転X
        m_angle.x = -0.1f;
        break;

    case Enemy::ACTION::ACT_ROLLP:   //左回りZ
        m_angle.z = 0.1f;
        break;

    case Enemy::ACTION::ACT_ROLLM:   //右回りZ
        m_angle.z = -1.0f;
        break;
    }

    //カウンター減算
    if (--m_actionKeepTime <= 0)
    {
        //乱数により次のアクションと持続時間設定
        m_action = static_cast<ACTION>(rand4(m_mt));
        m_actionKeepTime = rand10(m_mt2);
    }

}

