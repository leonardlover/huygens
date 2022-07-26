#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "errors.c"

#define WIDTH 1280
#define HEIGHT 720

SDL_Texture *loadTexture(char *path, SDL_Renderer *rend)
{
	SDL_Surface *surf = IMG_Load(path);

	if (!surf)
		return NULL;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);

	return tex;
}

int cursorOverRect(SDL_Rect rect, int x, int y)
{
	if (rect.x <= x && x <= rect.x + rect.w &&
	    rect.y <= y && y <= rect.y + rect.h)
			return 1;

	return 0;
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

	SDL_Texture *title = loadTexture("media/img/title.png", rend);

	if (!title)
		return textureError(rend, win);

	SDL_Rect pos;

	if (SDL_QueryTexture(title, NULL, NULL, &pos.w, &pos.h) != 0)
		return queryError(title, rend, win);

	pos.w *= 3;
	pos.h *= 3;
	pos.x = (WIDTH - pos.w) / 2;
	pos.y = -pos.h;

	SDL_Texture *stars[20];
	SDL_Rect starsPos[20];

	for (int i = 0; i < 20; i++) {
		stars[i] = loadTexture("media/img/star.png", rend);

		if (!stars[i])
			return textureError(rend, win);

		if (SDL_QueryTexture(stars[i], NULL, NULL, &starsPos[i].w, &starsPos[i].h) != 0)
			return queryError(stars[i], rend, win);

		starsPos[i].w *= 5;
		starsPos[i].h *= 5;
		starsPos[i].x = rand() % WIDTH;
		starsPos[i].y = -starsPos[i].h - rand() % HEIGHT;
	}

	SDL_Texture *button = loadTexture("media/img/heart.png", rend);

	if (!button)
		return textureError(rend, win);

	SDL_Rect buttonPos;

	if (SDL_QueryTexture(button, NULL, NULL, &buttonPos.w, &buttonPos.h) != 0)
		return queryError(button, rend, win);

	buttonPos.w *= 8;
	buttonPos.h *= 8;
	buttonPos.x = (WIDTH - buttonPos.w) / 2;
	buttonPos.y = (HEIGHT - buttonPos.h) / 2;

	int mouseX, mouseY, leftClick;

	int inGame = 0;

	int quit = 0;

	while (!quit) {
		SDL_Event event;

		leftClick = 0;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftClick = 1;
			default:
				break;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		if (cursorOverRect(buttonPos, mouseX, mouseY) && leftClick)
			inGame = 1;
		if (!inGame) {
			pos.y += 200 / 60;

			if (pos.y >= pos.h / 2)
				pos.y = pos.h / 2;

			for (int i = 0; i < 20; i++) {
				starsPos[i].y += 150 / 60;

				if (starsPos[i].y >= HEIGHT)
					starsPos[i].y = HEIGHT;
			}

			SDL_RenderClear(rend);
			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, title, NULL, &pos);
			SDL_RenderCopy(rend, button, NULL, &buttonPos);
		
			for (int i = 0; i < 20; i++)
				SDL_RenderCopy(rend, stars[i], NULL, &starsPos[i]);

			SDL_RenderPresent(rend);
		} else {
			SDL_RenderClear(rend);
			SDL_RenderPresent(rend);
		}

		SDL_Delay(1000 / 60);
	}

	for (int i = 0; i < 20; i++)
		SDL_DestroyTexture(stars[i]);

	SDL_DestroyTexture(button);
	SDL_DestroyTexture(title);
	SDL_DestroyTexture(bg);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
