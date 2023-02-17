#pragma once
#include	"gameobject.h"
#include	"CModel.h"
#include	"dx11mathutil.h"
#include	"CDirectInput.h"

class Player :public GameObject {
private:
	CModel	*m_model;	// ‚R‚cƒ‚ƒfƒ‹

public:
	bool Init();

	void Draw();

	void DrawWithAxis();

	void Update();

	void Finalize();

	static Player& GetInstance() {
		static Player Instance;
		return Instance;
	}

	const CModel* GetModel() const{
		return m_model;
	}

	void SetModel(CModel* pmodel) {
		m_model = pmodel;
	}
};