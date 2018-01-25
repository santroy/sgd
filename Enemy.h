#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Enemy{
	private:
		int height;
		int width;
	public:
		int x;
		int y;
		bool destroyed;
		Enemy(int h=50, int w=50, int ox=360, int oy=220);
		~Enemy();
		int increaseX();
		int decreaseX();
		int increaseY();
		void free();
};
