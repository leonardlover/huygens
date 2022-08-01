#ifndef UTILS_C
#define UTILS_C


#include <stdio.h>
#include <stdlib.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "utils.h"


#ifndef WIDTH
#define WIDTH 1260
#endif


#ifndef HEIGHT
#define HEIGHT 700
#endif


void printError(enum ErrorType *error)
{
	char *errorMsg;

	switch (*error) {
	case INITIALIZATION:
		errorMsg = "Initialization";
		break;
	case MEMORY:
		errorMsg = "Memory";
		break;
	case WINDOW:
		errorMsg = "Window";
		break;
	case RENDERER:
		errorMsg = "Renderer";
		break;
	case SURFACE:
		errorMsg = "Surface";
		break;
	case TEXTURE:
		errorMsg = "Texture";
		break;
	case QUERY:
		errorMsg = "Query";
		break;
	default:
		break;
	}

	printf("%s error: %s\n", errorMsg, SDL_GetError());
}


struct GameAssets *createGameAssets(char *gameName, enum ErrorType *error)
{
	struct GameAssets *assets = calloc(1, sizeof(struct GameAssets));

	if (!assets) {
		*error = MEMORY;
		return NULL;
	}

	assets->window = SDL_CreateWindow(gameName,
					  SDL_WINDOWPOS_CENTERED,
					  SDL_WINDOWPOS_CENTERED,
					  WIDTH,
					  HEIGHT,
					  0);

	if (!assets->window) {
		*error = WINDOW;
		return NULL;
	}

	assets->renderer = SDL_CreateRenderer(assets->window,
					      -1,
					      SDL_RENDERER_ACCELERATED |
					      SDL_RENDERER_PRESENTVSYNC);

	if(!assets->renderer) {
		*error = RENDERER;
		return NULL;
	}

	return assets;
}


void _destroyAsset(struct Asset *asset)
{
	SDL_DestroyTexture(asset->texture);
	free(asset->rect);
	free(asset);
}


void clearAssetData(struct GameAssets *assets)
{
	for (size_t i = 0 ; i < assets->count; i++)
		_destroyAsset(assets->assetData[i]);

	assets->count = 0;
}


void killGame(struct GameAssets *assets)
{
	if(assets->assetData) {
		clearAssetData(assets);
		free(assets->assetData);
	}

	if (assets->renderer)
		SDL_DestroyRenderer(assets->renderer);

	if (assets->window)
		SDL_DestroyWindow(assets->window);

	if (assets)
		free(assets);

	SDL_Quit();
}


int raiseError(enum ErrorType *error, struct GameAssets *assets)
{
	printError(error);
	killGame(assets);
	
	return 1;
}


SDL_Texture *_loadTexture(char *path, struct GameAssets *assets, enum ErrorType *error)
{
	SDL_Surface *surf = IMG_Load(path);

	if (!surf) {
		*error = SURFACE;
		return NULL;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(assets->renderer, surf);
	SDL_FreeSurface(surf);

	if (!tex) {
		*error = TEXTURE;
		return NULL;
	}

	return tex;
}


struct Asset *loadAsset(char *path, struct GameAssets *assets, enum ErrorType *error)
{
	SDL_Texture *tex = _loadTexture(path, assets, error);

	if (!tex)
		return NULL;

	SDL_Rect *rect = calloc(1, sizeof(SDL_Rect));

	if (!rect) {
		SDL_DestroyTexture(tex);
		*error = MEMORY;
		return NULL;
	}

	if (SDL_QueryTexture(tex, NULL, NULL, &rect->w, &rect->h) != 0) {
		SDL_DestroyTexture(tex);
		free(rect);
		*error = QUERY;
		return NULL;
	}

	struct Asset *asset = malloc(sizeof(struct Asset));

	if(!asset) {
		SDL_DestroyTexture(tex);
		free(rect);
		*error = MEMORY;
		return NULL;
	}

	asset->texture = tex;
	asset->rect = rect;

	if (assets->buffer == 0) {
		assets->assetData = malloc(sizeof(struct Asset *));

		if (!assets->assetData) {
			_destroyAsset(asset);
			*error = MEMORY;
			return NULL;
		}

		assets->buffer = 1;
	} else if (assets->count == assets->buffer) {
		assets->buffer *= 2;
		struct Asset **reallocatedAssetData = realloc(assets->assetData,
							      assets->buffer * sizeof(struct Asset *));

		if (!reallocatedAssetData) {
			clearAssetData(assets);
			*error = MEMORY;
			return NULL;
		}

		assets->assetData = reallocatedAssetData;
	}
		
	assets->assetData[assets->count] = asset;
	(assets->count)++;

	return asset;
}


void renderAssetData(struct GameAssets *assets, int present)
{
	if (assets->assetData) {
		SDL_RenderClear(assets->renderer);

		for (size_t i = 0; i < assets->count; i++)
			SDL_RenderCopy(assets->renderer,
				       (assets->assetData[i])->texture,
				       NULL,
				       (assets->assetData[i])->rect);

		if (present)
			SDL_RenderPresent(assets->renderer);
	}
}


#endif
