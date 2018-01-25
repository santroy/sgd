#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Texture{
	private:
		int mWidth;
		int mHeight;

	public:
		SDL_Texture* mTexture;
		Texture();
		~Texture();
		bool loadFromFile(std::string path, SDL_Renderer* rnd);
		void free();
		int getWidth();
		int getHeight();
};
