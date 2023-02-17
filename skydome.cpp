#include    "skYdome.h"
#include    "dx11mathutil.h"

bool Skydome::Init()
{
    //s—ñ‰Šú‰»
    DX11MtxIdentity(m_mtx);

    return true;
}

void Skydome::Draw()
{
    //ƒ‚ƒfƒ‹•`‰æ
    m_Skydomemodel->Draw(m_mtx);
}

void Skydome::Update()
{
}

void Skydome::Finalize()
{
    m_Skydomemodel->Uninit();
}
