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

	SDL_Texture *playButton = loadTexture("media/img/playButton.png", rend);

	if (!playButton)
		return raiseError("Texture", assets);

	if (associateTexture(playButton, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect playButtonPos;

	if (SDL_QueryTexture(playButton, NULL, NULL, &playButtonPos.w, &playButtonPos.h) != 0)
		return raiseError("Query", assets);

	playButtonPos.w *= 1.5;
	playButtonPos.h *= 1.5;
	playButtonPos.x = (WIDTH - playButtonPos.w) / 5;
	playButtonPos.y = (HEIGHT - playButtonPos.h) * 3 / 5;

	SDL_Texture *skinsButton = loadTexture("media/img/skinsButton.png", rend);

	if (!skinsButton)
		return raiseError("Texture", assets);

	if (associateTexture(skinsButton, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect skinsButtonPos;

	if (SDL_QueryTexture(skinsButton, NULL, NULL, &skinsButtonPos.w, &skinsButtonPos.h) != 0)
		return raiseError("Query", assets);

	skinsButtonPos.w *= 1.5;
	skinsButtonPos.h *= 1.5;
	skinsButtonPos.x = (WIDTH - skinsButtonPos.w) * 4 / 5;
	skinsButtonPos.y = (HEIGHT - skinsButtonPos.h) * 3 / 5;

	SDL_Texture *optionsButton = loadTexture("media/img/optionsButton.png", rend);

	if (!optionsButton)
		return raiseError("Texture", assets);

	if (associateTexture(optionsButton, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect optionsButtonPos;

	if (SDL_QueryTexture(optionsButton, NULL, NULL, &optionsButtonPos.w, &optionsButtonPos.h) != 0)
		return raiseError("Query", assets);

	optionsButtonPos.w *= 1.5;
	optionsButtonPos.h *= 1.5;
	optionsButtonPos.x = (WIDTH - optionsButtonPos.w) / 5;
	optionsButtonPos.y = (HEIGHT - optionsButtonPos.h) * 4 / 5;

	SDL_Texture *quitButton = loadTexture("media/img/quitButton.png", rend);

	if (!quitButton)
		return raiseError("Texture", assets);

	if (associateTexture(quitButton, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect quitButtonPos;

	if (SDL_QueryTexture(quitButton, NULL, NULL, &quitButtonPos.w, &quitButtonPos.h) != 0)
		return raiseError("Query", assets);

	quitButtonPos.w *= 1.5;
	quitButtonPos.h *= 1.5;
	quitButtonPos.x = (WIDTH - quitButtonPos.w) * 4 / 5;
	quitButtonPos.y = (HEIGHT - quitButtonPos.h) * 4 / 5;

	int mouseX, mouseY, leftClick;

	int inGame = 0;
	int hltd = 0;
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

			if (mouseOverRect(playButtonPos, mouseX, mouseY)) {
				if (!hltd) {
					highlightRect(&playButtonPos, 1.1);
					hltd = 1;
				}
			} else {
				if (hltd) {
					highlightRect(&playButtonPos, 1 / 1.1);
					hltd = 0;
				}
			}

			if (mouseOverRect(quitButtonPos, mouseX, mouseY) && leftClick)
				quit = 1;

			SDL_RenderClear(rend);
			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, title, NULL, &titleRect);
			SDL_RenderCopy(rend, playButton, NULL, &playButtonPos);
			SDL_RenderCopy(rend, skinsButton, NULL, &skinsButtonPos);
			SDL_RenderCopy(rend, optionsButton, NULL, &optionsButtonPos);
			SDL_RenderCopy(rend, quitButton, NULL, &quitButtonPos);
		
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
