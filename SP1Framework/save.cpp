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

void save::saveState(string posx, string posy, string charstate) {
	ofstream Map;
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
	Map.open("save.txt");
	savePos = posx + posy + charstate;
	Map << savePos;
	Map.close();
	cout << savePos << endl;
}

void save::saveState(string posx, string posy, string charstate, string projx, string projy) {
	ofstream Map;
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
	if (projx.length() == 1) {
		projx = "0" + projx;
	}
	if (projy.length() == 1) {
		projy = "0" + projy;
	}
	Map.open("save.txt");
	savePos = posx + posy + charstate + projx + projy;
	//if (savePos.length() != 12)
	//	savePos = "191400000000";
	Map << savePos;
	Map.close();
}

void save::defaultSave() {
	ofstream Map;
	Map.open("save.txt");
	Map << "201400000000";
	Map.close();
}

void save::readSave() {
	string line;
	ifstream myfile("save.txt");
	if (myfile.is_open()) {
		getline(myfile, line);
		cout << line << '\n';
		myfile.close();
	}
}

int save::loadSave() {
	string line; string saveline;
	ifstream map("save.txt");
	string posx; string posy; string projx; string projy; string charstate;
	if (map.is_open()) {
		while (getline(map, line));
		saveline = line;
		if (saveline.length() == 8)
			saveline = saveline + "00";
		if (saveline.length() < 10)
			saveline = "201400000000";
		posx = saveline.substr(0, 2);
		posy = saveline.substr(2, 2);
		charstate = saveline.substr(4, 2);
		projx = saveline.substr(6, 2);
		projy = saveline.substr(8, 2);
		x = stoi(posx);
		y = stoi(posy);
		projxx = stoi(projx);
		projyy = stoi(projy);
	}
	return 0;
}

int save::returnX() {
	return x;
}

int save::returnY() {
	return y;
}

int save::returnProjX() {
	return projxx;
}

int save::returnProjY() {
	return projyy;
}

int save::returnCharState() {
	bool chars = true;
	if (state == 1)
		chars = false;
	return chars;
}

int main1() {
	save state;
	state.defaultSave();
	state.loadSave();
	cout << state.returnX() << endl;
	cout << state.returnY() << endl;
	cout << state.returnProjX() << endl;
	cout << state.returnProjY() << endl;
	return 0;
}