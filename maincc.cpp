
#include <iostream>
#include "cleanup.h"
#include "drawing_functions.h"
#include "res_path.h"
#include "SDL_mixer.h"



using namespace std;

int main(int argc, char** argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cout << "Something went wrong! " << SDL_GetError() << endl;
	}

	SDL_Window* window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_OPENGL);
	if (window == nullptr) {
		SDL_Quit();
		cout << "Something also went wrong here" << endl;
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {

		cleanup(window);
		SDL_Quit();
		cout << "renderer error" << endl;
		return 1;
	}

	SDL_RenderSetLogicalSize(renderer, 1280, 720);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		cout << "sdl image did not initialise" << endl;
		return 1;
	}

	if (TTF_Init() != 0) {
		SDL_Quit();
		cout << "sdl ttf did not initialise" << endl;
		return 1;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_Quit();
		cout << "mixer did not initialize" << endl;
		return 1;
	}

	string resPath = getResourcePath();
	SDL_Texture* texture = loadTexture(resPath + "space_background.png", renderer);

	while (SDL_GetTicks() < 5000) {
	
		SDL_RenderClear(renderer);
		renderTexture(texture, renderer, 0, 0);
		SDL_RenderPresent(renderer);

	}


	cleanup(renderer);
	cleanup(window);
	cleanup(texture);

	SDL_Quit();

	return 0;

}