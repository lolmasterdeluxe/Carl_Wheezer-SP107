#include <fstream>
#include <iostream>
#include <string>
#include "Framework/console.h"
#include "game.h"
#include "save.h"

using namespace std;

int x = 0; int y = 0; int state = 0;

void save(string posx, string posy) {
	ofstream Map;
	string savePos;
	if (posx.length() == 1) {
		posx = "0" + posx;
	}
	if (posy.length() == 1) {
		posy = "0" + posy;
	}
	Map.open("save.txt");
	savePos = posx + posy;
	Map << savePos;
	Map.close();
	cout << savePos << endl;
}

void defaultSave() {
	ofstream Map;
	Map.open("save.txt");
	Map << "1914";
	Map.close();
}

void readSave() {
	string line;
	ifstream myfile("save.txt");
	if (myfile.is_open()) {
		getline(myfile, line);
		cout << line << '\n';
		myfile.close();
	}
}

int loadSave() {
	string line;
	string saveline;
	ifstream map("save.txt");
	ofstream read;
	string posx; string posy; string status;
	if (map.is_open()) {
		while (getline(map, line));
		saveline = line;
		posx = saveline.substr(0, 2);
		posy = saveline.substr(2, 4);
		x = stoi(posx);
		y = stoi(posy);
	}
	return 0;
}

int returnX() {
	return x;
}

int returnY() {
	return y;
}