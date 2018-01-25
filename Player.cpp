#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "player.h"


Player::Player(int x, int y, int h, int w){
	xPos=x;
	yPos=y;
	height=h;
	width=w;
	distance=10;
}

Player::~Player(){
	free();
}

int Player::increaseX(){
	xPos+=distance;
	return xPos;
}

int Player::decreaseX(){
	xPos-=distance;
	return xPos;
}

void Player::free(){
	xPos=0;
	yPos=0;
	height=0;
	width=0;
	distance=0;
}
