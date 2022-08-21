#include "./gameEngine.h"

gameEngine::gameEngine() {

    fps = 60;

    for (int i = 0; i < 300; i++) {
        framelist[i] = 1 / fps;
    }

    window = nullptr;
    windowID = NULL;
    renderer = nullptr;
    assetSurface = nullptr;
    assetTexture = nullptr;
    gameRunning = false;

    timeRunning = 0;
    frames = 0;

    gWidth = 0;
    gHeight = 0;
    wWidth = 0;
    wHeight = 0;
    pixelSize = 1;
    spriteSize = 8;

    spriteRect.h = spriteSize;
    spriteRect.w = spriteSize;
    spriteRect.x = 0;
    spriteRect.y = 0;

    assetRect.h = 8;
    assetRect.w = 8;
    assetRect.x = 0;
    assetRect.y = 0;

    ef = 0;

    keyboardstate = nullptr;
    contoller = nullptr;
    mouseWheelX = 0;
    mouseWheelY = 0;

}

gameEngine::~gameEngine() {}

// Creates a sdl window
bool gameEngine::createWindow(int wW, int wH, float ps) {

    setPixelSize(ps);

    // get game resoultion
    gWidth = wW / ps;
    gHeight = wH / ps;

    // ajust window size based on pixel size
    wWidth = wW;
    wHeight = wH;

    spriteRect.w = 8 * pixelSize;
    spriteRect.h = 8 * pixelSize;

    // if init faild exit with error
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)) return false;

    window = SDL_CreateWindow(
        "The Survivlest",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        wWidth,                            // width, in pixels
        wHeight,                           // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
    if (window == nullptr) {
        return false;
    }
    else {
        windowID = SDL_GetWindowID(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    keyboardstate = SDL_GetKeyboardState(NULL);

    return true;
}

// sets up game
void gameEngine::start() {
    timeRunning = SDL_GetTicks64();

    gameRunning = true;

    initGame();

    SDL_RenderPresent(renderer);
    frames = 1;

    int indexframe = 0;
    ef = 0;

    // run update function in a while loop
    while (gameRunning) {

        updateGame();
        getInput();

        SDL_RenderPresent(renderer);
        frames++;

        // caculates delay for target framerate
        //std::cout << " cal time ms: " << SDL_GetTicks64() - timeRunning;
        Uint64 spin = 0;
        while (SDL_GetTicks64() <= timeRunning + (1000.0f / fps)) { 
            SDL_Delay(1); 
            spin++; 
        }
        //std::cout << " Spins: " << spin << " frames: " << frames << " ef: " << ef << '\n';

        if (indexframe < 300) {
            framelist[indexframe] = float(SDL_GetTicks64() - timeRunning) / 1000;
            indexframe++;
        }
        else {
            indexframe = 0;
        }

        double temp = 0;
        for (int i = 0; i < 300; i++) {
            temp += framelist[i];
        }

        ef = temp / 300.0; // divide by 300 for average

        timeRunning = SDL_GetTicks64();
    }

    stop();
}

// clean up memory before game closes
void gameEngine::stop() {

    gameRunning = false;
    if (assetTexture != NULL) SDL_DestroyTexture(assetTexture);
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}

void gameEngine::getInput() {

    if (keyboardstate[SDL_SCANCODE_ESCAPE]) {
        stop();
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_WINDOWEVENT: {
            if (event.window.windowID == windowID) {
                switch (event.window.event) {

                case SDL_WINDOWEVENT_CLOSE: {
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
                    break;
                }
                }
            }
            break;
        }

        case SDL_MOUSEWHEEL: {

            mouseWheelY = event.wheel.y;

        }
        break;

        case SDL_QUIT: {
            gameRunning = false;
        }
        }
    }

}

void gameEngine::setPixelSize(float size) {

    pixelSize = size;
    spriteSize = size * 8.0f;


}

void gameEngine::clearRenderer() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, NULL);
    SDL_RenderClear(renderer);

}

void gameEngine::drawSprite(int index, int posX, int posY, int spriteRez, float spriteSizeFinal, SDL_RendererFlip flip) {

    if (posX > -spriteSize * spriteSizeFinal && posX < wWidth && posY > -spriteSize * spriteSizeFinal && posY < wHeight) {

        spriteRect.x = posX;
        spriteRect.y = posY;
        spriteRect.w = spriteSize * spriteSizeFinal;
        spriteRect.h = spriteSize * spriteSizeFinal;

        assetRect.x = (index % 32) * 8;
        assetRect.y = (index / 32) * 8;
        assetRect.w = spriteRez * 8;
        assetRect.h = spriteRez * 8;

        //SDL_RenderCopy(renderer, assetTexture, &assetRect, &spriteRect);

        SDL_RenderCopyEx(renderer, assetTexture, &assetRect, &spriteRect, 0, NULL, flip);

    }
}

void gameEngine::drawText(std::string text, int textPosX, int textPosY, float size, int spacing) {

    spriteRect.x = textPosX;
    spriteRect.y = textPosY;
    spriteRect.w = size * spriteSize;
    spriteRect.h = size * spriteSize;

    for (int i = 0; i <= text.length(); i++) {

        assetRect.x = 8 * (text[i] + 896);
        assetRect.y = 0;

        while (assetRect.x > 256) {
            assetRect.x -= 256;
            assetRect.y += 8;
        }

        SDL_RenderCopy(renderer, assetTexture, &assetRect, &spriteRect);
        spriteRect.x = spriteRect.x + spriteRect.w + spacing * spriteSize;
    }

}

bool gameEngine::initGame() {
    return true;
}

bool gameEngine::updateGame() {
    return true;
}

