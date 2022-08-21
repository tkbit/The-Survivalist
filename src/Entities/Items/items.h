#pragma once

#include "../entity.h"


enum class itemID
{
	unknown,
	porkchop,
	slimeball
};

class item : public entity {
public:
	item();

public:
	itemID itemId;

public:
	void update() override;
	virtual void draw();
};

class slimeball : public item {
public:
	slimeball();


};