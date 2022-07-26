#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#define WIDTH 1280
#define HEIGHT 720

#define SPEED 300

void printError(char *prompt)
{
	printf("%s: %s\n", prompt, SDL_GetError());
}

int initError(void)
{
	printError("Initialization error");
	return 1;
}

int windowError(void)
{
	printError("Window error");
	SDL_Quit();
	return 1;
}

int rendererError(SDL_Window *win)
{
	printError("Renderer error");
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 1;
}

int textureError(SDL_Renderer *rend, SDL_Window *win)
{
	printError("Texture error");
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 1;
}

SDL_Texture *loadTexture(char *path, SDL_Renderer *rend)
{
	SDL_Surface *surf = IMG_Load(path);

	if (!surf)
		return NULL;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);

	return tex;
}

int main(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return initError();

	SDL_Window *win = SDL_CreateWindow("Huygens",
					   SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WIDTH,
					   HEIGHT,
					   0);

	if (!win)
		return windowError();

	SDL_Renderer *rend = SDL_CreateRenderer(win,
						-1,
						SDL_RENDERER_ACCELERATED |
						SDL_RENDERER_PRESENTVSYNC);

	if (!rend)
		return rendererError(win);

	SDL_Texture *bg = loadTexture("media/img/bg.png", rend);

	if (!bg)
		return textureError(rend, win);

	int quit = 0;

	while (!quit) {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
			default:
				break;
			}
		}

		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, bg, NULL, NULL);
		SDL_RenderPresent(rend);

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyTexture(bg);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
