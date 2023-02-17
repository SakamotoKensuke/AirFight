#pragma once
#include	"gameobject.h"
#include	"CModel.h"

class Skydome : public GameObject
{
public:
	bool Init();

	void Draw();

	void Update();

	void Finalize();

	void SetModel(CModel* pmodel) {
		m_Skydomemodel = pmodel;
	}

private:
	CModel *m_Skydomemodel;
};