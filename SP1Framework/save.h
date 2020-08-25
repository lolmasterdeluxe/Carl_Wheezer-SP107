#pragma once
#include <string>
using namespace std;
class save {
private:
	int x = 0; int y = 0; int enemyX = 0; int enemyY = 0; int state = 0;
	int bossX = 0; int bossY = 0; int hp = 0; int mana = 0;
public:
	save();
	~save();
	void saveState(string posx, string posy, string charstate, string health, string mana, string eX, string eY, string bX, string bY);
	void saveState();
	void readSave();
	void defaultSave();
	int loadSave();
	int returnX();
	int returnY();
	bool returnCharState();
	int returnEnemyX();
	int returnEnemyY();
	int returnBossX();
	int returnBossY();
	int returnCharHealth();
	int returnCharMana();
	string returnCharSave();
	string returnEnemySave();
};