#include "player.h"

#include "../../game.h"

player::player() {

	name = "player";
	index = 0;
	animateOrgin = index;
	animationFrames = 8;
	spriteSize = 3;
	vec = { 0,3 };
	velX = 0;
	velY = 0;
	maxSpeed = 12;
	timmer2 = 0;
	entityListPos = 0;

	for (int i = 0; i < 10; i++) {
		invetory[i] = 0;
	}

}

void player::update() {

	animate(300);

#define ks g->keyboardstate

	bool doMove = false;
	velX = 0;
	velY = 0;
	float dz = 0.15;
	float jsX = (SDL_GameControllerGetAxis(g->contoller, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0);
	float jsY = (SDL_GameControllerGetAxis(g->contoller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0);

	float tempMaxSpeed = maxSpeed;

	if (g->currentMap->getTileData((int)posX + 1, (int)posY + 1) == 129) {
		tempMaxSpeed = maxSpeed / 2;
	}

	if (ks[SDL_SCANCODE_UP] || ks[SDL_SCANCODE_W] || SDL_GameControllerGetButton(g->contoller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
		//animateOrgin = 0;
		velY = -tempMaxSpeed;
		vec.DirX = M_PI / 2;
	}
	else if (ks[SDL_SCANCODE_DOWN] || ks[SDL_SCANCODE_S] || SDL_GameControllerGetButton(g->contoller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
		//animateOrgin = 2;
		velY = tempMaxSpeed;
		vec.DirX = (3 * M_PI) / 2;
	}
	if (ks[SDL_SCANCODE_LEFT] || ks[SDL_SCANCODE_A] || SDL_GameControllerGetButton(g->contoller, SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
		//animateOrgin = 4;
		velX = -tempMaxSpeed;
		vec.DirX = M_PI;
	}
	else if (ks[SDL_SCANCODE_RIGHT] || ks[SDL_SCANCODE_D] || SDL_GameControllerGetButton(g->contoller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
		//animateOrgin = 6;
		velX = tempMaxSpeed;
		vec.DirX = M_PI * 2;
	}
	else if (jsX > dz || jsX < -dz || jsY > dz || jsY < -dz) {

		if (jsY < 0 && jsX < 0.5 && jsX > -0.5) {
			//animateOrgin = 0;
			vec.DirX = M_PI / 2;
		}
		else if (jsY > 0 && jsX < 0.5 && jsX > -0.5) {
			//animateOrgin = 2;
			vec.DirX = (3 * M_PI) / 2;
		}
		else if (jsX < 0 && jsY < 0.5 && jsY > -0.5) {
			//animateOrgin = 4;
			vec.DirX = M_PI;
		}
		else if (jsX > 0 && jsY < 0.5 && jsY > -0.5) {
			//animateOrgin = 6;
			vec.DirX = M_PI * 2;
		}

		velX = jsX * tempMaxSpeed;
		velY = jsY * tempMaxSpeed;


		//trying to find angle of controler stick. may need fixed later
		vec.DirX = std::atan2(jsY, jsX);
		if (vec.DirX < 0) {
			vec.DirX = vec.DirX * -1;
		}
		//std::cout << vec.DirX * 180 / M_PI << '\n';
	}

	moveVel();

	bool actionButton = ks[SDL_SCANCODE_SPACE] || SDL_GameControllerGetButton(g->contoller, SDL_CONTROLLER_BUTTON_X);

	if (!actionButton) {
		timmer2 = SDL_GetTicks64();
	}

	if (actionButton && SDL_GetTicks64() - lapTime >= 200 && SDL_GetTicks64() - timmer2 < 200) {
		lapTime = SDL_GetTicks64();

		creature* c;
		for (int i = 1; i < g->entityListSize; i++) {
			if (g->entityList[i] != nullptr && g->entityList[i]->type == 1) {
				c = (creature*)g->entityList[i];
				if (std::sqrt((c->posX - posX) * (c->posX - posX) + (c->posY - posY) * (c->posY - posY)) < 2) { //if with in a certion distance...

					//subtract health and knockback creature
					c->health -= 1;

					if (vec.DirX < (3 * M_PI) / 2 && vec.DirX > M_PI / 2) {
						c->posX -= 1;
					}
					else {
						c->posX += 1;
					}
				}
			}
		}
	}

	item* c;
	for (int i = 1; i < g->entityListSize; i++) {
		if (g->entityList[i] != nullptr && g->entityList[i]->type == 2) {
			c = (item*)g->entityList[i];
			if (std::sqrt((c->posX - posX - 1) * (c->posX - posX - 1) + (c->posY - posY - 1) * (c->posY - posY - 1)) < 1) { //if with in a certion distance...

				if (g->entityList[i]->type == 2) {

					if (invetoryAdd((int)(c->itemId))) {
						delete g->entityList[i];
						g->entityList[i] = nullptr;
					}
					else
					{
						std::cout << "Invetory Full!\n";
					}
					
				}

			}
		}
	}

	if (health <= 0) {
		g->entityList[entityListPos] = nullptr;
		delete this;
	}

}

bool player::invetoryAdd(int itemId, int amount) {

	bool foundSpace = false;

	for (int i = 0; i < 20; i += 2) {
		if (invetory[i] == 0 || invetory[i] == itemId) {
			invetory[i] = itemId;

			if (!(invetory[i + 1] > 4)) {
				invetory[i + 1] += amount;
				foundSpace = true;
				break;
			}

		}
	}

	/*for (int i = 0; i < 20; i += 1) {
		std::cout << invetory[i] << ' ';
	}
	std::cout << '\n';*/

	return foundSpace;
}