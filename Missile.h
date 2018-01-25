#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Missile{
	private:
		int height, width;
	public:
		int x, y;
		bool shot;
		Missile(int x=5, int y=0, int h=15, int w=15);
		int decreaseY();
		void free();
		~Missile();
};
