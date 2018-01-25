#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Missile.h"

Missile::Missile(int x, int y, int h, int w){
	this->x = x;
	this->y = y;
	height = h;
	width = w;
	shot = false;
}

Missile::~Missile(){
	free();
}
int Missile::decreaseY(){
	y = y - 10;
	return y;
}

void Missile::free(){
	this->x = 0;
	this->y = 0;
	this->height = 0;
	this->width = 0;
}
