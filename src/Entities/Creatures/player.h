#pragma once

#include "../entity.h"
#include "../Items/items.h"

class player : public creature {
public:
	player();

public:
	float maxSpeed;
	int invetory[20];

private:
	Uint64 timmer2;

public:
	//animates the player with delay between frames in millsecounds
	void update() override;
	bool invetoryAdd(int itemId, int amount = 1);
};