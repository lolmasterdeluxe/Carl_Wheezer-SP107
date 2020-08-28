#include <fstream>
#include <iostream>
#include <string>
#include "Framework/console.h"
#include "game.h"
#include "save.h"

using namespace std;

string charSaveState = "0"; string enemySaveState = "0";

save::save() {
}
save::~save() {
}

void save::saveState(string s) {
	ofstream Map;
	Map.open("save.txt");
	Map << s;
	Map.close();
}

void save::defaultSave() {
	ofstream Map;
	Map.open("save.txt");
	Map << "201500200000\n30282628";
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

int save::loadSave() {
	string line; string saveline;
	ifstream map("save.txt");
	string chr, posx, posy, charstate, health, mn, ex[10], ey[10], eh[10], l, bx, by, bh;
	if (map.is_open()) {
		while (getline(map, line)) {
			saveline = line;
			chr = saveline.substr(0, 1);
			posx = saveline.substr(1, 2);
			posy = saveline.substr(3, 2);
			health = saveline.substr(5, 3);
			mn = saveline.substr(8, 3);
			l = saveline.substr(11, 1);
			charstate = saveline.substr(12, 1);

			for (int i = 0; i < 10; i++) {
				for (int k = 13; k < 73; k+=6) {
					ex[i] = saveline.substr(k, 2);
					ey[i] = saveline.substr(k+2, 2);
					eh[i] = saveline.substr(k+4, 2);
				}
			}

			bx = saveline.substr(73, 2);
			by = saveline.substr(75, 2);
			bh = saveline.substr(77, 3);
		}
		x = stoi(posx);
		y = stoi(posy);
		hp = stoi(health);
		mana = stoi(mn);
		state = stoi(charstate);
		level = stoi(l);
		ch = stoi(chr);

		for (int i = 0; i < 10; i++) {
			enemyX[i] = stoi(ex[i]);
			enemyY[i] = stoi(ey[i]);
			enemyH[i] = stoi(eh[i]);
		}

		bossX = stoi(bx);
		bossY = stoi(by);
		bossH = stoi(bh);
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

int save::returnEnemyX(int n) {
	return enemyX[n];
}

int save::returnEnemyY(int n) {
	return enemyY[n];
}

int save::returnEnemyH(int n) {
	return enemyH[n];
}

bool save::returnCharState() {
	bool chars = false;
	if (state == 1)
		chars = true;
	return chars;
}

int save::returnBossX() {
	return bossX;
}

int save::returnBossY() {
	return bossY;
}

int save::returnBossH() {
	return bossH;
}

int save::returnChar() {
	return ch;
}

int save::returnLevel() {
	return level;
}

int main1() {
	save state;

	state.loadSave();
	cout << "Char x = " << state.returnX() << " Char y = " << state.returnY() << " Char state = " << state.returnCharState() << " Char = " << state.returnChar() << endl;
	cout << "Char health = " << state.returnCharHealth() << " Char mana = " << state.returnCharMana() << " Level = " << state.returnLevel() << endl;
	for (int i = 0; i < 10; i++) {
		cout << "EnemyX(" << i << ") = " << state.returnEnemyX(i) << " EnemyY(" << i << ") = " << state.returnEnemyY(i) << " EnemyH(" << i << ") = " << state.returnEnemyH(i)<< endl;
	}
	cout << "Boss x = " << state.returnBossX() << " Boss y = " << state.returnBossY() << " Boss H = " << state.returnBossH() << endl;

	return 0;
}