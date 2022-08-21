#pragma once

#include<SDL.h>

#include "GameEngine/gameEngine.h"
#include "maps.h"
#include "Entities/entity.h"
#include "Entities/Items/items.h"
#include "Entities/Items/item_porkchop.h"
#include "Entities/Creatures/player.h"

class game : public gameEngine {
public:
    map* currentMap = nullptr;
    float cameraX, cameraY;
    static const int entityListSize = 1024;
    entity* entityList[entityListSize];

    int select;

    bool spawn(entityID id, float x, float y);

protected:
    bool initGame();
    bool updateGame();

};