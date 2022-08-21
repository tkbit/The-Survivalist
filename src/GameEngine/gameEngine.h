#pragma once

#include<iostream>
#include<string>

#include <SDL.h>

struct vf2d {

	float DirX, MagY;

};

class gameEngine {

public:
	gameEngine();
	~gameEngine();

public:
	float fps;
	SDL_Window* window;
	Uint32 windowID;
	SDL_Renderer* renderer;
	SDL_Surface* assetSurface;
	SDL_Texture* assetTexture;
	SDL_Rect spriteRect, assetRect;
	Sint32 mouseWheelX, mouseWheelY;
	const Uint8* keyboardstate;
	SDL_GameController* contoller;
	Uint64 timeRunning;
	Uint64 frames;
	int gWidth, gHeight, wWidth, wHeight;
	float spriteSize;
	float ef;


private:
	float pixelSize;
	bool gameRunning;
	float framelist[300];


public:
	bool createWindow(int wWidth, int wHeight, float pixelSize = 6.0f);
	void start();
	void stop();
	void getInput();
	void setPixelSize(float size);

	void clearRenderer();
	void drawSprite(int index = 1, int posX = 0, int posY = 0, int spriteRez = 1, float spriteSizeFinal = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawText(std::string text, int textPosX, int textPosY, float size = 1, int spacing = 0);

	virtual bool initGame();
	virtual bool updateGame();



};