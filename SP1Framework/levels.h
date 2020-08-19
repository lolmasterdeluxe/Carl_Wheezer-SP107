#pragma once
#include "game.h"
#include "Framework/console.h"

class levels {
private:
	int numberOfEnemies;
public:
	levels();
	~levels();
	//void loadLevelData(int number);
	void renderLevel();
	int returnEnemies();
};

//struct SGameChar
//{
//	COORD m_cLocation;
//	double m_dHealth;
//	double m_dMana;
//	bool m_bActive;
//};


