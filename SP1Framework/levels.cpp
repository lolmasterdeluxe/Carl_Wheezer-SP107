#include "levels.h"
#include <iostream>
#include <fstream>
#include <string>

//Console g_Console(45, 15, "Ninjas, monsters n' robots");

//SGameChar   g_sChar;
//SGameChar   g_sProj;
//SGameChar   g_sEnemy;

levels::levels() {
}

levels::~levels() {
}

//void levels::loadLevelData(int number) {
//	int n = 0;
//	std::string levelFile;
//	std::string line; std::string perLine;
//	if (number == 1)
//		levelFile = "levelone.txt";
//	std::ifstream level(levelFile);
//	if (level.is_open()) {
//		while (std::getline(level, line)) {
//			perLine = line;
//			for (int i = 0; i < perLine.length(); i++) {
//				if (perLine[i] == '.') {
//					perLine[i] = ' ';
//				}
//				if (perLine[i] == 'P') {
//					;
//				}
//			}
//			n++;
//		}
//	}
//}

void levels::renderLevel() {
}

int levels::returnEnemies() {
	return numberOfEnemies;
}

//int main1() {
//	levels one;
//	one.loadLevelData(1);
//	return 0;
//}
