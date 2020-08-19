#pragma once
#include "game.h"
#include "Framework/console.h"

class levels {
private:
	int numberOfEnemies;
public:
	levels();
	~levels();
	void levelData();
	void renderLevel();
	int returnEnemies();
};

