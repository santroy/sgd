#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "Texture.h"
#include "Enemy.h"
#include "Player.h"
#include "Missile.h"
#include "string"
#include <time.h>

using namespace std;


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Texture texture;

TTF_Font * arial;
SDL_Color font_color = { 255,255,255 };
SDL_Surface * surface;

SDL_Rect rect;

Mix_Chunk* shoot = NULL;
Mix_Chunk* kill = NULL;
Mix_Chunk* music = NULL;

int TCM = SDL_GetTicks();
int TCS = SDL_GetTicks();
int TCSS = SDL_GetTicks();

bool MusicPlaying = false;

int startFPS;
int endFPS;
int diffFPS;
float delay;
int fps = 24;

void fps_start() {
	startFPS = clock();
}

void fps_koniec() {
	endFPS = clock();
	diffFPS = endFPS - startFPS;
	delay = (1000.0 / fps) - diffFPS;
	if (delay > 0) {
		SDL_Delay(delay);
	}
}

bool init() {

	bool done = false;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		SDL_Quit();
		cout << "Something went wrong! " << SDL_GetError() << endl;
		return done;
	}


	if (TTF_Init() != 0) {
		SDL_Quit();
		cout << "Error occured while importing fonts!" << endl;
		return 1;
	}
	arial = TTF_OpenFont("fonts/arial.ttf", 20);


	window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

	if (window == nullptr) {
		SDL_Quit();
		cout << "Error occured while creating new window!" << endl;
		return done;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_Quit();
		cout << "Error occured while creating renderer!" << endl;
		return done;
	}

	int imgFlags = IMG_INIT_PNG;
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		SDL_Quit();
		cout << "Error occured while initializing images!" << endl;
		return done;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		SDL_Quit();
		cout << "Error occured while initializing sounds!" << endl;
		return done;
	}

	return done = true;

}
bool loadMedia(string path) {

	bool success = true;
	if (!texture.loadFromFile(path.c_str(), renderer)) {
		cout << "Could not load texture!\n";
		success = false;
	}

	shoot = Mix_LoadWAV("sound/shoot.wav");
	kill = Mix_LoadWAV("sound/hit.wav");
	music = Mix_LoadWAV("sound/music.wav");
	if (shoot == NULL || kill == NULL || music == NULL) {
		cout << "Failed to load sound effect! SDL_mixer Error: %s\n" << Mix_GetError();
		success = false;
	}


	Mix_Volume(1, MIX_MAX_VOLUME / 12);
	Mix_Volume(2, MIX_MAX_VOLUME / 1);	
	Mix_Volume(3, MIX_MAX_VOLUME / 5);	
	return success;
}


void render(SDL_Renderer* rnd, SDL_Rect object, int yPos, int xPos) {
	object.y = yPos;
	object.x = xPos;
	SDL_RenderSetViewport(rnd, &object);
	SDL_RenderCopy(rnd, texture.mTexture, NULL, NULL);
}


void hit(int row, Enemy objectInv[][5], Missile missileObj[], int &score, int &countDest) {
	for (int i = 0; i<10; i++) {
		for (int j = 0; j<row; j++) {

			if (missileObj[i].shot == true) {
				for (int k = 0; k<row; k++) {

					if (!objectInv[j][k].destroyed) {

						if (missileObj[i].y >= (objectInv[j][k].y) && missileObj[i].y <= (objectInv[j][k].y + 20) && missileObj[i].x >= (objectInv[j][k].x) && missileObj[i].x <= (objectInv[j][k].x + 20)) {
							score += 100;

							countDest++;

							objectInv[j][k].destroyed = true;

							Mix_PlayChannel(3, kill, 0);
							missileObj[i].shot = false;
						}
					}
				}
			}
		}
	}
}


void close() {
	texture.free();
	Mix_FreeChunk(shoot);
	Mix_FreeChunk(kill);
	shoot = NULL;
	kill = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {

	int score = 0;

	if (!init()) {
		SDL_Quit();
		cout << "General Error occured!";
		return 1;
	}
	
		if (!loadMedia("textures/enemy.png")) {
			cout << "Failed to load texture\n";
		}
		else {
			int const row = 5;					
			int countDestroyed = 0;		
			int counter = 0;			
			int frames = 0;				
			int mod = 1;					
			int timer = 10;				
			int timerSpeed = 10;		
			int distanceX = 80;		
			int distanceY = 40;		
			Player player = Player(SCREEN_WIDTH / 2 - 40,SCREEN_HEIGHT-60,40,40);	
			Missile missile[10];		
			Enemy invader[5][5];	
			const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

			for (int i = 0; i<row; i++) {
				for (int j = 0; j<row; j++) {
					invader[i][j].y -= (distanceY * i);
					invader[i][j].x -= (distanceX * j);
				}
			}
			bool quit = false;
			bool endOfGame = false;
			bool stepDown = false;

			SDL_Event e;

			while (!quit) {
				//if (TCS + 1 < SDL_GetTicks()) {
					fps_start();

					if (!MusicPlaying) {
						Mix_PlayChannel(1, music, -1);
						MusicPlaying = true;
					}

					if (!endOfGame) {

						if (countDestroyed == (row * row)) {
							countDestroyed = 0;

							if (mod > 1) {
								mod--;
							}
							else {
								mod = 1;
							}

							for (int i = 0; i < 10; i++) {
								missile[i].shot = false;
								missile[i].y = 0;
								missile[i].x = 0;
							}

							for (int i = 0; i < row; i++) {
								for (int j = 0; j < row; j++) {
									invader[i][j].destroyed = false;
									invader[i][j].y = 220 - (distanceY*i);
									invader[i][j].x = 340 - (distanceX*j);
								}
							}
						}


						while (SDL_PollEvent(&e) != 0) {

							if (keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]) {
								if (timer == timerSpeed) {
									if (counter >= 9) {
										counter = 0;
									}

									Mix_PlayChannel(2, shoot, 0);
									missile[counter].shot = true;
									missile[counter].x = player.xPos;
									missile[counter].y = player.yPos - 5;
									counter++;
									timer = 0;
								}
							}

							if (keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]) {
								if (player.xPos > 20) {
									player.decreaseX();
								}
							}

							if (keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]) {
								if (player.xPos < SCREEN_WIDTH - 50) {
									player.increaseX();
								}
							}


							if (e.type == SDL_QUIT) {
								quit = true;
							}

							else if (e.type == SDL_KEYDOWN) {
								switch (e.key.keysym.sym) {

									/*						case SDLK_SPACE:
															case SDLK_w:
															case SDLK_UP:
																if (timer == timerSpeed) {
																	if (counter >= 9) {
																		counter = 0;
																	}

																	Mix_PlayChannel(2, shoot, 0);
																	missile[counter].shot = true;
																	missile[counter].x = player.xPos;
																	missile[counter].y = player.yPos - 5;
																	counter++;
																	timer = 0;
																}
																break;*/

																//case SDLK_a:
																//case SDLK_LEFT:
																//	if (player.xPos>20) {
																//		player.decreaseX();
																//	}
																//	break;

																//case SDLK_d:
																//case SDLK_RIGHT:
																//	if (player.xPos<SCREEN_WIDTH - 50) {
																//		player.increaseX();
																//	}
																//	break;
								}
							}
						}



						if (timer < timerSpeed) {
							timer++;
						}

						SDL_Delay(8);


						SDL_RenderClear(renderer);
						frames++;


						if (frames%mod == 0) {

							if (!stepDown) {
								for (int i = 0; i < row; i++) {
									for (int j = 0; j < row; j++) {
										if (invader[i][j].destroyed != true) {

											if (invader[i][j].x >= SCREEN_WIDTH - 50 - (distanceX*j)) {
												stepDown = true;
												invader[i][j].x = SCREEN_WIDTH - 50 - (distanceX*j);
												invader[i][j].increaseY();
												invader[i][j].decreaseX();
											}
											else {
												invader[i][j].increaseX();
											}

											if (invader[i][j].y >= player.yPos) {
												endOfGame = true;
												break;
											}
										}
									}
								}
							}

							else {
								for (int i = 0; i < row; i++) {
									for (int j = 0; j < row; j++) {

										if (invader[i][j].destroyed != true) {
											if (invader[i][j].x <= 340 - (distanceX*j)) {
												stepDown = false;
												invader[i][j].x = 340 - (distanceX*j);
												invader[i][j].increaseY();
												invader[i][j].increaseX();
											}
											else {
												invader[i][j].decreaseX();
											}

											if (invader[i][j].y >= player.yPos) {
												endOfGame = true;
												break;
											}
										}
									}

									if (endOfGame == true) {
										break;
									}
								}
							}
						}

						for (int i = 0; i < 10; i++) {
							if (missile[i].shot == true) {
								if (missile[i].y > 0) {
									missile[i].decreaseY();
								}

								else {
									missile[i].shot = false;
								}
							}
						}
						hit(row, invader, missile, score, countDestroyed);


						texture.loadFromFile("textures/space_background.png", renderer);
						rect.w = 1280;
						rect.h = 720;
						render(renderer, rect, 0, 0);
						SDL_RenderCopy(renderer, texture.mTexture, NULL, NULL);

						texture.loadFromFile("textures/enemy.png", renderer);
						for (int i = 0; i < row; i++) {
							for (int j = 0; j < row; j++) {
								if (invader[i][j].destroyed != true) {
									rect.h = 40;
									rect.w = 40;
									render(renderer, rect, invader[i][j].y, invader[i][j].x);
								}
							}
						}

						texture.loadFromFile("textures/missile.png", renderer);
						for (int i = 0; i < 10; i++) {
							if (missile[i].shot == true) {
								rect.h = 20;
								rect.w = 20;
								render(renderer, rect, missile[i].y, missile[i].x);
							}
						}


						texture.loadFromFile("textures/player.png", renderer);
						rect.h = 40;
						rect.w = 40;
						render(renderer, rect, player.yPos, player.xPos);

						SDL_RenderCopy(renderer, texture.mTexture, NULL, NULL);


						surface = TTF_RenderText_Blended(arial, to_string(score).c_str(), font_color);

						render(renderer, rect, 0, 0);
						SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, surface), NULL, NULL);


						SDL_RenderPresent(renderer);
					}


					else {
						SDL_RenderClear(renderer);
						for (int i = 0; i < 10; i++) {
							missile[i].free();
						}
						for (int i = 0; i < row; i++) {
							for (int j = 0; j < row; j++) {
								invader[i][j].free();
							}
						}
						player.free();
						SDL_RenderClear(renderer);
						break;
					}
				
				//TCS = SDL_GetTicks();
				fps_koniec();
			}
		}
	
	int leaderboards[10];
	for (int i = 0; i<10; i++) {
		leaderboards[i] = 0;
	}

	fstream writeScore;

	writeScore.open("Leaderboards.txt", ios::out | ios::app);
	if (writeScore.is_open()) {
		writeScore << score << "\n";
		cout << "Your score: " << score << "\n";
	}
	writeScore.close();

	fstream getLeadearboards;
	getLeadearboards.open("Leaderboards.txt", ios::in);

	if (getLeadearboards.is_open()) {
		while (getLeadearboards.eof() != true) {
			getLeadearboards >> score;
			for (int i = 0; i<10; i++) {
				if (score>leaderboards[i]) {
					for (int j = 9; j>i; j--) {
						leaderboards[j] = leaderboards[j - 1];
					}
					leaderboards[i] = score;
					score = 0;
				}
			}
		}
	}

	for (int i = 0; i<10; i++) {
		cout << i + 1 << ". " << leaderboards[i] << "\n";
	}
	getLeadearboards.close();
	close();
	//system("pause");
	return 0;
}
