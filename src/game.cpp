#include"game.h"

/*
To do:

*   beter orgnize code and add more comits for refrence
        -create enum for sprite sheet or change how sprites are loaded

*   get items textures and sprites all working and conected properly

*   Add more animales like cows and monsters to fight
        -find sprites for said creatures

*   add abilty to collect resouirces from diging and mineing

*   add tress

*   add invetory and crafting system

*   setup git project for better versioning and tracking

*/


// called once on start up
bool game::initGame() {
    
    //fps = 144;

    setPixelSize(8);

    select = 0;

    cameraX = 0;
    cameraY = 0;

    clearRenderer();
    
    currentMap = new map_Start(this); // load starter map

    for (int i = 0; i < entityListSize; i++) {
        entityList[i] = nullptr;
    }

    spawn(entityID::player, currentMap->width / 2, currentMap->height / 2); // spawns player at entityListSize[0]

    for (int i = 0; i < 50; i++) {
        spawn(entityID::pig, rand() % currentMap->width, rand() % currentMap->width);
    }

    for (int i = 0; i < 10; i++) {
        spawn(entityID::slime, rand() % currentMap->width, rand() % currentMap->width);
    }

    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            std::cout << "Found Controller: " << SDL_GameControllerNameForIndex(i) << '\n';
            contoller = SDL_GameControllerOpen(i);
        }
        else {
            SDL_Log("Index \'%i\' is not a compatible controller.", i);
        }
    }

    return true;
}

//####################################################################################################################################################

// called every frame
bool game::updateGame() {

    player* p = (player*)entityList[0];

    //if m key pressed spawn pigs
    if (keyboardstate[SDL_SCANCODE_M]) {
        spawn(entityID::pig, p->posX, p->posY);
    }
    
    //entity update loop------------------------------------------------------------------------------------------------------------------------------
    for (int i = entityListSize - 1; i >= 0; i--) {
        if (entityList[i] != nullptr) {

            //keep enties within the boundrys of the map
            if (entityList[i]->posX < 0) entityList[i]->posX = 0;
            if (entityList[i]->posX > currentMap->width - 2) entityList[i]->posX = currentMap->width - 2;
            if (entityList[i]->posY < 0) entityList[i]->posY = 0;
            if (entityList[i]->posY > currentMap->height - 2) entityList[i]->posY = currentMap->height - 2;

            entityList[i]->update(); //update after border check because entity could delete its self in update

        }
    }
    
    //keep camera on player---------------------------------------------------------------------------------------------------------------------------
    cameraX = entityList[0]->posX - ((wWidth / 2) / spriteSize) + 1;
    cameraY = entityList[0]->posY - ((wHeight / 2) / spriteSize) + 1;

    //stop camera from moving out of world border
    if (cameraX > currentMap->width - wWidth / spriteSize) {
        cameraX = (currentMap->width - wWidth / spriteSize) - 0.0f;
    }
    if (cameraX < 0) {
        cameraX = 0.0f;
    }
    if (cameraY > currentMap->height - wHeight / spriteSize) {
        cameraY = (currentMap->height - wHeight / spriteSize) - 0.0f;
    }
    if (cameraY < 0) {
        cameraY = 0.0f;
    }
    
    //draw map---------------------------------------------------------------------------------------------------------------------------------------
    currentMap->draw();

    //draw entites-----------------------------------------------------------------------------------------------------------------------------------
    for (int i = entityListSize - 1; i >= 0; i--) {
        if (entityList[i] != nullptr) {
            entityList[i]->draw();
        }
    }

    int hbY = wHeight - spriteSize * 1.5;

    //draw hotbar------------------------------------------------------------------------------------------------------------------------------------
    for (int i = 0; i < 10; i++) {

        int hbX = (wWidth / 2) + (i * spriteSize) - (5 * spriteSize);

        drawSprite(192, hbX, hbY);

        if (i == select) {
            drawSprite(193, (wWidth / 2) + (select * spriteSize) - (5 * spriteSize), hbY);
        }

        //draw items in hotbar
        if (p->invetory[i * 2] > 0) {
            drawSprite(159 + p->invetory[i * 2], hbX, hbY);
            drawText(std::to_string(p->invetory[(i * 2) + 1]), hbX, hbY, 0.3);
        }

        //move slected hotbar tile to keyboard number pressed
        if (keyboardstate[30 + i]) {
            select = i;
        }

        //mouse wheel affects select....
        select -= mouseWheelY;
        mouseWheelY = 0;
        //...and clamp it down
        if (select > 9) {
            select = 0;
        }
        else if(select < 0) {
            select = 9;
        }

    }

    return true;
}

bool game::spawn(entityID id, float x, float y) {

    bool found = false;

    for (int i = 0; i < entityListSize; i++) {

        found = false;

        if (entityList[i] == nullptr) {

            switch (id)
            {
            case entityID::player :
                entityList[i] = new player;
                found = true;
                break;

            case entityID::pig :
                entityList[i] = new pig;
                found = true;
                break;

            case entityID::slime:
                entityList[i] = new slime;
                found = true;
                break;

            case entityID::item_slimeball:
                entityList[i] = new slimeball;
                found = true;
                break;

            case entityID::item :
                entityList[i] = new item;
                found = true;
                break;

            case entityID::item_porkchop:
                item* itemptr = new item;
                itemptr->itemId = itemID::porkchop;
                entityList[i] = itemptr;
                found = true;
                break;

            }

            if (found) {
                entityList[i]->g = this;
                entityList[i]->entityListPos = i;
                entityList[i]->posX = x;
                entityList[i]->posY = y;
                return true;
            }
        }
    }

    return false;

}