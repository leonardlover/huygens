#ifndef UTILS_H
#define UTILS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


struct Asset {
	SDL_Texture *texture;
	SDL_Rect *rect;
};


struct GameAssets {
	SDL_Window *window;
	SDL_Renderer *renderer;

	struct Asset **assetData;
	size_t count;
	size_t buffer;
};


enum ErrorType {
	INITIALIZATION,
	MEMORY,
	WINDOW,
	RENDERER,
	SURFACE,
	TEXTURE,
	QUERY,
};


void printError(enum ErrorType *error);


struct GameAssets *createGameAssets(char *gameName, enum ErrorType *error);


void _destroyAsset(struct Asset *asset);


void clearAssetData(struct GameAssets *assets);


void killGame(struct GameAssets *assets);


int raiseError(enum ErrorType *error, struct GameAssets *assets);


SDL_Texture *_loadTexture(char *path, struct GameAssets *assets, enum ErrorType *error);


struct Asset *loadAsset(char *path, struct GameAssets *assets, enum ErrorType *error);


void renderAssetData(struct GameAssets *assets, int present);


#endif
