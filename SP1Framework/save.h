#pragma once
#include <string>

class save {
private:
	int x = 0; int y = 0; int projxx = 0; int projyy = 0; int state = 0;
public:
	save();
	~save();
	void saveState(std::string posx, std::string posy, std::string charstate);
	void saveState(std::string posx, std::string posy, std::string charstate, std::string projx, std::string projy);
	void readSave();
	void defaultSave();
	int loadSave();
	int returnX();
	int returnY();
	int returnCharState();
	int returnProjX();
	int returnProjY();
};