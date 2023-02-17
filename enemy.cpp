#include "enemy.h"
#include "hommingmissile.h"
#include "drawaxis.h"


bool Enemy::Init()
{
    //�s�񏉊���
    DX11MtxIdentity(m_mtx);

    m_actionKeepTime = 10;

    m_speed = 1.0f;

    //�V�[�h�l����p����
    std::random_device rand_dev;

    //�V�[�h�l�Z�b�g
    m_mt.seed(rand_dev());
    m_mt2.seed(rand_dev());

    return true;
}

void Enemy::Draw()
{
    //���f���`��
    m_model->Draw(m_mtx);
}

void Enemy::DrawWithAxis()
{
    //���f���`��
    drawaxis(m_mtx, 200, m_pos);
    m_model->Draw(m_mtx);
}

void Enemy::Update()
{
    XMFLOAT4 axisX;
    XMFLOAT4 axisY;
    XMFLOAT4 axisZ;

    //X�������o��
    axisX.x = m_mtx._11;
    axisX.y = m_mtx._12;
    axisX.z = m_mtx._13;
    axisX.w = 0.0f;

    //y�������o��
    axisY.x = m_mtx._21;
    axisY.y = m_mtx._22;
    axisY.z = m_mtx._23;
    axisY.w = 0.0f;

    //z�������o��
    axisZ.x = m_mtx._31;
    axisZ.y = m_mtx._32;
    axisZ.z = m_mtx._33;
    axisZ.w = 0.0f;

    XMFLOAT4 qt;

    //�s�񂩂�N�I�[�^�j�I���𐶐�
    DX11GetQtfromMatrix(m_mtx, qt);

    XMFLOAT4 qtx, qty, qtz;

    //�w�莲��]�̃N�I�[�^�j�I���𐶐�
    DX11QtRotationAxis(qtx, axisX, m_angle.x);
    DX11QtRotationAxis(qty, axisY, m_angle.y);
    DX11QtRotationAxis(qtz, axisZ, m_angle.z);

    //�N�I�[�^�j�I��������
    XMFLOAT4 tempqt1;
    DX11QtMul(tempqt1, qt, qtx);

    XMFLOAT4 tempqt2;
    DX11QtMul(tempqt2, qty, qtz);

    XMFLOAT4 tempqt3;
    DX11QtMul(tempqt3, tempqt1, tempqt2);

    //�N�I�[�^�j�I�����m�[�}���C�Y
    DX11QtNormalize(tempqt3, tempqt3);

    //�N�I�[�^�j�I������s����쐬
    DX11MtxFromQt(m_mtx, tempqt3);

    //z�������o��
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
    //��������
    std::uniform_int_distribution<int32_t> rand4(0, static_cast<int>(Enemy::ACTION::MAX_ACTION) - 1);
    std::uniform_int_distribution<int32_t> rand10(10, 50);
    //��]�p���������񃊃Z�b�g
    m_angle.x = m_angle.y = m_angle.z = 0.0f;

    //�A�N�V�����ɂ���]�p���Z�b�g
    switch (m_action)
    {
    case Enemy::ACTION::ACT_STRAIGHT:   //���i
        m_angle.x = m_angle.y = m_angle.z = 0.0f;
        break;

    case Enemy::ACTION::ACT_LEFT:   //����]Y
        m_angle.y = 0.1f;
        break;

    case Enemy::ACTION::ACT_RIGHT:   //�E��]Y
        m_angle.y = -0.1f;
        break;

    case Enemy::ACTION::ACT_UP:   //���]X
        m_angle.x = 0.1f;
        break;

    case Enemy::ACTION::ACT_DOWN:   //����]X
        m_angle.x = -0.1f;
        break;

    case Enemy::ACTION::ACT_ROLLP:   //�����Z
        m_angle.z = 0.1f;
        break;

    case Enemy::ACTION::ACT_ROLLM:   //�E���Z
        m_angle.z = -1.0f;
        break;
    }

    //�J�E���^�[���Z
    if (--m_actionKeepTime <= 0)
    {
        //�����ɂ�莟�̃A�N�V�����Ǝ������Ԑݒ�
        m_action = static_cast<ACTION>(rand4(m_mt));
        m_actionKeepTime = rand10(m_mt2);
    }

}

