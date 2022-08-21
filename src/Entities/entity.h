#pragma once

#include "../GameEngine/gameEngine.h"
//#include "../game.h"
#include "../maps.h"

class game;

enum class entityID
{

	player,
	pig,
	item,
	item_porkchop,
	slime,
	item_slimeball

};

class entity {

public:
	entity();

public:
	game* g;
	int entityListPos;
	const char* name;
	int type;
	int index, animateOrgin, animationFrames, spriteSize;
	float posX, posY;
	vf2d vec;

public:
	virtual void draw();
	virtual void update() {}

};



class creature : public entity {
public:
	creature();

public:
	float velX, velY, health, maxHealth;
	int lapTime;

public:
	void draw() override;
	virtual void animate(int delay);
	void moveVec();
	void moveVel();

};

class pig : public creature {
public:
	pig();

public:
	void update() override;
};

class slime : public creature {
public:
	slime();

public:
	void update() override;
};