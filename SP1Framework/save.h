#pragma once
#include <string>
using namespace std;
class save {
private:
	int x = 0; int y = 0; int enemyX[10]; int enemyY[10]; int enemyH[10]; int state = 0;
	int bossX = 0; int bossY = 0; int bossH = 0;  int hp = 0; int mana = 0; int level; int ch;
public:
	save();
	~save();
	void saveState(string s);
	void readSave();
	void defaultSave();
	int loadSave();
	int returnX();
	int returnY();
	bool returnCharState();
	int returnChar();
	int returnEnemyX(int n);
	int returnEnemyY(int n);
	int returnEnemyH(int n);
	int returnBossX();
	int returnBossY();
	int returnBossH();
	int returnCharHealth();
	int returnCharMana();
	int returnLevel();
};