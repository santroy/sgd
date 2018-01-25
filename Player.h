#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Player{
	private:
		int distance;
		int height;
		int width;
	public:
		int xPos;
		int yPos;
		Player(int,int,int,int);
		~Player();
		int increaseX();
		int decreaseX();
		void free();
};
