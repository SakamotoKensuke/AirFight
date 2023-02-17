#pragma once
#include <cinttypes>

enum class SCENE {	// ƒV[ƒ“‘JˆÚ
	TITLE,
	GAME,
	END
};


static SCENE now_scene;

static bool sceneflg = false;

void GameInit();
void GameInput(uint64_t dt);
void GameUpdate(uint64_t dt);
void GameRender(uint64_t dt);
void GameDispose();