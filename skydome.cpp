#include    "skYdome.h"
#include    "dx11mathutil.h"

bool Skydome::Init()
{
    //行列初期化
    DX11MtxIdentity(m_mtx);

    return true;
}

void Skydome::Draw()
{
    //モデル描画
    m_Skydomemodel->Draw(m_mtx);
}

void Skydome::Update()
{
}

void Skydome::Finalize()
{
    m_Skydomemodel->Uninit();
}
