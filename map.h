#pragma once
#include	"gameobject.h"
#include	"CModel.h"

class Map :public GameObject{
private:
	CModel	m_map;	// �R�c���f��

public:
	bool Init();

	void Draw();

	void Update();

	void Finalize();

};

