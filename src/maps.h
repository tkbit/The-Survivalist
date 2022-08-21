#pragma once

#include "GameEngine/gameEngine.h"
#include "../lib/FastNoiseLite.h"

class game;

class map {

public:
	map();
	~map();

public:
	game* g;

	int width, height;

	std::string name;

public:
	int getTileData(int x, int y);
	bool load(const char* filePath);
	void draw();

private:
	int* mapArray;


};

class map_Start : public map {

public:
	map_Start(game* game);

};

class map_2 : public map {

public:
	map_2();

};
