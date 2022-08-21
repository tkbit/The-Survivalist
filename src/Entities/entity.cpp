#include "entity.h"

#include "../game.h"

entity::entity() {

	name = "missingName";
	entityListPos = 1;
	type = 0;
	index = 0;
	animateOrgin = index;
	animationFrames = 1;
	spriteSize = 1;
	vec = { 0,0 };

	posX = 0;
	posY = 0;

	g = nullptr;

}

void entity::draw() {

	//draw entity at world possition instead of camera possition

	g->drawSprite(index, (posX - g->cameraX) * g->spriteSize, (posY - g->cameraY) * g->spriteSize, spriteSize);

}


creature::creature() {

	vec = { 0,1 };
	type = 1;

	health = 10;
	maxHealth = 10;
	index = 161;
	animateOrgin = index;
	spriteSize = 1;
	lapTime = 0;
	velX = 0;
	velY = 0;
}

void creature::draw() {

	//draw entity at world possition instead of camera possition

	SDL_RendererFlip rf = SDL_FLIP_NONE;

	if (vec.DirX < (3 * M_PI) / 2 && vec.DirX > M_PI / 2) {
		rf = SDL_FLIP_HORIZONTAL;
	}

	g->drawSprite(index, (posX - g->cameraX) * g->spriteSize, (posY - g->cameraY) * g->spriteSize, spriteSize, 2.0f, rf);

	

	if (health < maxHealth) {
		//Draw Health Bar
		SDL_FRect rect = { (posX - g->cameraX) * g->spriteSize,(posY - g->cameraY - 0.25) * g->spriteSize,2 * g->spriteSize,0.25 * g->spriteSize };
		SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, NULL);
		SDL_RenderFillRectF(g->renderer, &rect);
		rect.w = (health / maxHealth) * g->spriteSize * 2;
		SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, NULL);
		SDL_RenderFillRectF(g->renderer, &rect);
	}

}

void creature::animate(int delay) {

	index = animateOrgin + (SDL_GetTicks64() / delay % animationFrames) * spriteSize;

}

void creature::moveVec() {

	posY += std::sin(vec.DirX) * vec.MagY * g->ef;
	posX += std::cos(vec.DirX) * vec.MagY * g->ef;

}

void creature::moveVel() {

	posY += velY * g->ef;
	posX += velX * g->ef;

}

pig::pig() {

	name = "pig";
	health = 5;
	maxHealth = 5;
	vec.MagY = 0;
	vec.DirX = 0;
	index = 256;
	animateOrgin = index;
	spriteSize = 2;
	lapTime = 0;

}

void pig::update() {

	if ((SDL_GetTicks64() - lapTime) >= 3000) {
		lapTime = SDL_GetTicks64();

		if (g->currentMap->getTileData((int)posX + 1, (int)posY + 1) == 129) {
			health -= 1;
		}

		if (rand() % 4 == 1) {
			vec.MagY = -2;
		}
		else {
			vec.MagY = 0;
		}

		vec.DirX = ((rand() % 360) / 180.0f) * M_PI;

	}

	moveVec();

	if (health <= 0) {

		g->spawn(entityID::item_porkchop, posX + 1, posY + 1);

 		g->entityList[entityListPos] = nullptr;
		delete this;
	}

}

slime::slime() {

	name = "slime";
	health = 5;
	maxHealth = 5;
	vec.MagY = 0;
	vec.DirX = 0;
	index = 258;
	animateOrgin = index;
	spriteSize = 2;
	lapTime = 0;

}

void slime::update() {

	if ((SDL_GetTicks64() - lapTime) >= 3000) {
		lapTime = SDL_GetTicks64();

		if (g->currentMap->getTileData((int)posX + 1, (int)posY + 1) == 129) {
			health -= 1;
		}

		if (rand() % 4 == 1) {
			vec.MagY = -2;
		}
		else {
			vec.MagY = 0;
		}

		vec.DirX = ((rand() % 360) / 180.0f) * M_PI;

	}

	moveVec();

	if (health <= 0) {

		g->spawn(entityID::item_slimeball, posX + 1, posY + 1);

		g->entityList[entityListPos] = nullptr;
		delete this;
	}

}