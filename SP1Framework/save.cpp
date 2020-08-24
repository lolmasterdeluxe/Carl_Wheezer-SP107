#include <fstream>
#include <iostream>
#include <string>
#include "Framework/console.h"
#include "game.h"
#include "save.h"

using namespace std;

save::save() {
}
save::~save() {
}

void save::saveState(string posx, string posy, string charstate, string health, string mana) {
	string savePos;
	if (posx.length() == 1) {
		posx = "0" + posx;
	}
	if (posy.length() == 1) {
		posy = "0" + posy;
	}
	if (charstate.length() == 1) {
		charstate = "0" + charstate;
	}
	if (health.length() < 4) {
		for (int i = 0; i < health.length(); i++) {
			health = "0" + health;
		}
	}
	if (mana.length() == 1) {
		mana = "0" + mana;
	}
	charSaveState = posx + posy + charstate + health + mana;
	saveState();
	cout << "saved char!";
}

void save::saveState(string eX, string eY, string bX, string bY) {
	string savePos;
	if (eX.length() == 1) {
		eX = "0" + eX;
	}
	if (eY.length() == 1) {
		eY = "0" + eY;
	}
	if (bX.length() == 1) {
		bX = "0" + bX;
	}
	if (bY.length() == 1) {
		bY = "0" + bY;
	}
	enemySaveState = eX + eY + bX + bY;
	saveState();
	cout << "saved enemy!";
}

void save::saveState() {
	ofstream Map;
	string savePos;
	Map.open("save.txt");
	savePos = charSaveState + enemySaveState;
	Map << savePos;
	Map.close();
	cout << "saved all!";
}

void save::defaultSave() {
	ofstream Map;
	Map.open("save.txt");
	Map << "20290020005050292629";
	Map.close();
	cout << "default saved!";
}

void save::readSave() {
	string line;
	ifstream myfile("save.txt");
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			cout << line << '\n';
		}
		myfile.close();
	}
}

string save::returnCharSave() {
	return charSaveState;
}

string save::returnEnemySave() {
	return enemySaveState;
}

int save::loadSave() {
	string line; string saveline;
	ifstream map("save.txt");
	string posx, posy, charstate, health, mn, ex, ey, bx, by;
	if (map.is_open()) {
		getline(map, line);
		saveline = line;
		if (saveline.length() < 20)
			saveline = "20290020000050292629";
		posx = saveline.substr(0, 2);
		posy = saveline.substr(2, 2);
		charstate = saveline.substr(4, 2);
		health = saveline.substr(6, 4);
		mn = saveline.substr(10, 2);
		ex = saveline.substr(12, 2);
		ey = saveline.substr(14, 2);
		bx = saveline.substr(16, 2);
		by = saveline.substr(18, 2);

		x = stoi(posx);
		y = stoi(posy);
		enemyX = stoi(ex);
		enemyY = stoi(ey);
		hp = stoi(health);
		mana = stoi(mn);
		state = stoi(charstate);
		bossX = stoi(bx);
		bossY = stoi(by);
	}
	return 0;
}

int save::returnX() {
	return x;
}

int save::returnY() {
	return y;
}

int save::returnCharHealth() {
	return hp;
}

int save::returnCharMana() {
	return mana;
}

int save::returnEnemyX() {
	return enemyX;
}

int save::returnEnemyY() {
	return enemyY;
}

bool save::returnCharState() {
	bool chars = true;
	if (state == 1)
		chars = false;
	return chars;
}

int save::returnBossX() {
	return bossX;
}

int save::returnBossY() {
	return bossY;
}

int main1() {
	save state;
	ifstream s;

	s.open("save.txt");
	//if (!s)
		//state.defaultSave();

	state.loadSave();
	cout << "Char x = " << state.returnX() << " Char y = " << state.returnY() << endl;
	cout << "Char health = " << state.returnCharHealth() << " Char mana = " << state.returnCharMana() << endl;
	cout << "Enemy x = " << state.returnEnemyX() << " Enemy y = " << state.returnEnemyY() << endl;
	cout << "Boss x = " << state.returnBossX() << " Boss y = " << state.returnBossY() << endl;

	// Save char state
	state.saveState("25", "29", "0", "50", "0");
	// Save enemy state
	state.saveState("30", "29", "35", "29");
	return 0;
}