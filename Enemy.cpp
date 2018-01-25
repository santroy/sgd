#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Enemy.h"

Enemy::Enemy(int h, int w, int ox, int oy){
	height=h;
	width=w;
	x=ox;
	y=oy;
	destroyed=false;
}

Enemy::~Enemy(){
	free();
}

int Enemy::increaseX(){
	x+=7;
	return x;
}

int Enemy::decreaseX(){
	x-=7;
	return x;
}
int Enemy::increaseY(){
	y+=40;
	return y;
}

void Enemy::free(){
	height=0;
	width=0;
	x=0;
	y=0;
}

