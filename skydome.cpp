#include    "skYdome.h"
#include    "dx11mathutil.h"

bool Skydome::Init()
{
    //�s�񏉊���
    DX11MtxIdentity(m_mtx);

    return true;
}

void Skydome::Draw()
{
    //���f���`��
    m_Skydomemodel->Draw(m_mtx);
}

void Skydome::Update()
{
}

void Skydome::Finalize()
{
    m_Skydomemodel->Uninit();
}
