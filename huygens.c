#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "utils.c"

#define WIDTH 1280
#define HEIGHT 720

int main(void)
{
	struct GameAssets *assets = createGameAssets();

	if (!assets)
		return raiseError("Memory", assets);
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return raiseError("Initialization", assets);

	initAssets(assets);

	SDL_Window *win = SDL_CreateWindow("Huygens",
					   SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WIDTH,
					   HEIGHT,
					   0);

	if (!win)
		return raiseError("Window", assets);

	associateWindow(win, assets);

	SDL_Renderer *rend = SDL_CreateRenderer(win,
						-1,
						SDL_RENDERER_ACCELERATED |
						SDL_RENDERER_PRESENTVSYNC);

	if (!rend)
		return raiseError("Renderer", assets);

	associateRenderer(rend, assets);

	SDL_Texture *bg = loadTexture("media/img/bg.png", rend);

	if (!bg)
		return raiseError("Texture", assets);

	if (associateTexture(bg, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Texture *title = loadTexture("media/img/title.png", rend);

	if (!title)
		return raiseError("Texture", assets);

	if (associateTexture(title, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect titleRect;

	if (SDL_QueryTexture(title, NULL, NULL, &titleRect.w, &titleRect.h) != 0)
		return raiseError("Query", assets);

	titleRect.w *= 3;
	titleRect.h *= 3;
	titleRect.x = (WIDTH - titleRect.w) / 2;
	titleRect.y = -titleRect.h;

	/*SDL_Texture *stars[20];
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
	}*/

	SDL_Texture *playButton = loadTexture("media/img/playButton.png", rend);

	if (!playButton)
		return raiseError("Texture", assets);

	if (associateTexture(playButton, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect playButtonPos;

	if (SDL_QueryTexture(playButton, NULL, NULL, &playButtonPos.w, &playButtonPos.h) != 0)
		return raiseError("Query", assets);

	playButtonPos.w *= 2;
	playButtonPos.h *= 2;
	playButtonPos.x = (WIDTH - playButtonPos.w) / 2;
	playButtonPos.y = (HEIGHT - playButtonPos.h) / 2;

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

		if (mouseOverRect(playButtonPos, mouseX, mouseY) && leftClick)
			inGame = 1;
		if (!inGame) {
			titleRect.y += 200 / 60;

			if (titleRect.y >= titleRect.h / 2)
				titleRect.y = titleRect.h / 2;

			/*for (int i = 0; i < 20; i++) {
				starsPos[i].y += 150 / 60;

				if (starsPos[i].y >= HEIGHT)
					starsPos[i].y = HEIGHT;
			}*/

			SDL_RenderClear(rend);
			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, title, NULL, &titleRect);
			SDL_RenderCopy(rend, playButton, NULL, &playButtonPos);
		
			//for (int i = 0; i < 20; i++)
				//SDL_RenderCopy(rend, stars[i], NULL, &starsPos[i]);

			SDL_RenderPresent(rend);
		} else {
			SDL_RenderClear(rend);
			SDL_RenderPresent(rend);
		}

		SDL_Delay(1000 / 60);
	}

	killGame(assets);

	return 0;
}
