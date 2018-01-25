#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "texture.h"

Texture::Texture(){
	mTexture=NULL;
	mWidth=0;
	mHeight=0;
}

Texture::~Texture(){
	free();
}


bool Texture::loadFromFile(std::string path, SDL_Renderer* rnd){

	free();
	SDL_Texture* newTexture=NULL;
	SDL_Surface* loadedSurface=IMG_Load(path.c_str());
	if(loadedSurface==NULL){
		std::cout << "Could not load image %s! SDL_Image Error: %s\n" << IMG_GetError();
	}
	else{

		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
		newTexture=SDL_CreateTextureFromSurface(rnd, loadedSurface);
		if(newTexture==NULL){
			std::cout << "Could not create texture! SDL Error: %s\n" << SDL_GetError();
		}
		else{
			mWidth=loadedSurface->w;
			mHeight=loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture=newTexture;
	return mTexture!=NULL;
}

void Texture::free(){
	if(mTexture!=NULL){
		SDL_DestroyTexture(mTexture);
		mTexture=NULL;
		mWidth=0;
		mHeight=0;
	}
}

int Texture::getWidth(){
	return mWidth;
}

int Texture::getHeight(){
	return mHeight;
}
