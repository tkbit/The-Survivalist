#include "maps.h"

#include "game.h"

map::map() {

	width = 0;
	height = 0;
	name = "NoName";
	g = nullptr;
	mapArray = nullptr;

}

map::~map() {

	delete[] mapArray;

}

int map::getTileData(int x, int y) {

	if (x >= 0 && x < width && y >= 0 && y < height)
		return mapArray[y * width + x];
	else
		return 0;

}

bool map::load(const char* filePath) {

	//load sprite sheet into memory
	g->assetSurface = SDL_LoadBMP(filePath);
	g->assetTexture = SDL_CreateTextureFromSurface(g->renderer, g->assetSurface);
	SDL_FreeSurface(g->assetSurface);

	mapArray = new int[width * height];

	// Create and configure FastNoise object
	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFrequency(0.09);

	// Gather noise data
	float* noiseData = new float[width * height];
	int index = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			noiseData[index++] = noise.GetNoise((float)x, (float)y);
		}
	}

	// Do something with this data...

	

	for (int i = 0; i < width * height; i++) {

		mapArray[i] = 129;//water

		if (noiseData[i] > -0.3) {
			mapArray[i] = 224 + (rand() % 8);//grass
		}

		if (noiseData[i] > 0.35) {
			mapArray[i] = 130;//stone
		}

	}

	// Free data later
	delete[] noiseData;

	return true;
}

void map::draw() {

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			g->drawSprite(getTileData(x, y), (x - g->cameraX) * g->spriteSize, (y - g->cameraY) * g->spriteSize);

		}
	}

}

map_Start::map_Start(game* game) {

	g = game;

	width = 100;
	height = 100;

	load("assets/spriteSheet.bmp");

}

map_2::map_2() {

	//load("");

}