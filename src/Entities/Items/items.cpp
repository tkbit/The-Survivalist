
#include "items.h"
#include "../../game.h"

item::item() {


	name = "unnamed";
	index = 959; // '?'
	type = 2;
	itemId = itemID::unknown;

}

slimeball::slimeball() {

	name = "slimeball";
	//index = 162;
	itemId = itemID::slimeball;

}

void item::update() {}

void item::draw() {

	//draw entity at world possition instead of camera possition

	g->drawSprite(159 + (int)itemId, (posX - g->cameraX) * g->spriteSize, (posY - g->cameraY) * g->spriteSize, spriteSize);

}