#include "../game.h"

int main(int argc, char* argv[]) {

	game* g = new game();

	if (g->createWindow(1280, 720)) {
		g->start();
		delete g;
		return 0;
	}

	delete g;
	return 1;
}