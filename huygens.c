#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#define WIDTH 1280
#define HEIGHT 720
#define HLFACTOR 1.1

#include "utils.c"

int main(void)
{
	/* WHEN GAME IS DONE DECLARE *MOST* VARIABLES HERE */

	/* Create game assets, a manager for game data */

	struct GameAssets *assets = createGameAssets();

	if (!assets)
		return raiseError("Memory", assets);
	
	/* Initialize SDL */

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return raiseError("Initialization", assets);

	initAssets(assets);

	/* Create game window */

	SDL_Window *win = SDL_CreateWindow("Huygens",
					   SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WIDTH,
					   HEIGHT,
					   0);

	if (!win)
		return raiseError("Window", assets);

	associateWindow(win, assets);

	/* Create renderer */

	SDL_Renderer *rend = SDL_CreateRenderer(win,
						-1,
						SDL_RENDERER_ACCELERATED |
						SDL_RENDERER_PRESENTVSYNC);

	if (!rend)
		return raiseError("Renderer", assets);

	associateRenderer(rend, assets);

	/* Load background */

	SDL_Texture *bg = loadTexture("media/img/bg.png", rend);

	if (!bg)
		return raiseError("Texture", assets);

	if (associateTexture(bg, assets) != 0)
		return raiseError("Memory", assets);

	/* Load game name */

	SDL_Texture *title = loadTexture("media/img/title.png", rend);

	if (!title)
		return raiseError("Texture", assets);

	if (associateTexture(title, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect titleRect;

	if (loadRect(title, &titleRect) != 0)
		return raiseError("Query", assets);
	
	/* Load buttons background */

	SDL_Texture *buttonBG = loadTexture("media/img/buttonBG.png", rend);

	if (!buttonBG)
		return raiseError("Texture", assets);

	if (associateTexture(buttonBG, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect playButtonBG, skinsButtonBG, optionsButtonBG, quitButtonBG;

	if(loadRect(buttonBG, &playButtonBG) != 0)
		return raiseError("Query", assets);
	if(loadRect(buttonBG, &skinsButtonBG) != 0)
		return raiseError("Query", assets);
	if(loadRect(buttonBG, &optionsButtonBG) != 0)
		return raiseError("Query", assets);
	if(loadRect(buttonBG, &quitButtonBG) != 0)
		return raiseError("Query", assets);

	/* Load play text */

	SDL_Texture *playText = loadTexture("media/img/playText.png", rend);

	if (!playText)
		return raiseError("Texture", assets);

	if (associateTexture(playText, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect playTextRect;

	if (loadRect(playText, &playTextRect) != 0)
		return raiseError("Query", assets);

	/* Load skins text */

	SDL_Texture *skinsText = loadTexture("media/img/skinsText.png", rend);

	if (!skinsText)
		return raiseError("Texture", assets);

	if (associateTexture(skinsText, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect skinsTextRect;

	if (loadRect(skinsText, &skinsTextRect) != 0)
		return raiseError("Query", assets);

	/* Load options text */

	SDL_Texture *optionsText = loadTexture("media/img/optionsText.png", rend);

	if (!optionsText)
		return raiseError("Texture", assets);

	if (associateTexture(optionsText, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect optionsTextRect;

	if (loadRect(optionsText, &optionsTextRect) != 0)
		return raiseError("Query", assets);

	/* Load quit text */

	SDL_Texture *quitText = loadTexture("media/img/quitText.png", rend);

	if (!quitText)
		return raiseError("Texture", assets);

	if (associateTexture(quitText, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect quitTextRect;

	if (loadRect(quitText, &quitTextRect) != 0)
		return raiseError("Query", assets);

	/* Set rectangles positions and sizes */

	scaleRect(&titleRect, 3);
	centerXRect(&titleRect);
	setYRect(&titleRect, titleRect.h / 2);

	scaleRect(&playButtonBG, 0.75);
	setXPropRect(&playButtonBG, 0.2);
	setYPropRect(&playButtonBG, 0.6);
	
	scaleRect(&skinsButtonBG, 0.75);
	setXPropRect(&skinsButtonBG, 0.8);
	setYPropRect(&skinsButtonBG, 0.6);

	scaleRect(&optionsButtonBG, 0.75);
	setXPropRect(&optionsButtonBG, 0.2);
	setYPropRect(&optionsButtonBG, 0.8);

	scaleRect(&quitButtonBG, 0.75);
	setXPropRect(&quitButtonBG, 0.8);
	setYPropRect(&quitButtonBG, 0.8);

	moveRectToRect(&playTextRect, &playButtonBG);
	moveRectToRect(&skinsTextRect, &skinsButtonBG);
	moveRectToRect(&optionsTextRect, &optionsButtonBG);
	moveRectToRect(&quitTextRect, &quitButtonBG);

	/* Set up game loop variables WILL BE DEPRECATED */

	int quit = 0;

	enum CurrentMenu menu = MAIN_MENU;
	enum HighlightedButton hltd = NONE;

	int mouseX, mouseY;
	int leftClick = 0;
	int inGame = 0;

	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftClick = 1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (leftClick)
					leftClick = 0;
			default:
				break;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		switch (menu) {
		case MAIN_MENU:
			switch (hltd) {
			case NONE:
				if (mouseOverRect(playButtonBG, mouseX, mouseY)) {
					highlightRect(&playButtonBG, HLFACTOR);
					hltd = PLAY;
				} else if (mouseOverRect(skinsButtonBG, mouseX, mouseY)) {
					highlightRect(&skinsButtonBG, HLFACTOR);
					hltd = SKINS;
				} else if (mouseOverRect(optionsButtonBG, mouseX, mouseY)) {
					highlightRect(&optionsButtonBG, HLFACTOR);
					hltd = OPTIONS;
				} else if (mouseOverRect(quitButtonBG, mouseX, mouseY)) {
					highlightRect(&quitButtonBG, HLFACTOR);
					hltd = QUIT;
				}
				break;
			case PLAY:
				if (!mouseOverRect(playButtonBG, mouseX, mouseY)) {
					highlightRect(&playButtonBG, 1 / HLFACTOR);
					hltd = NONE;
				}
				break;
			case SKINS:
				if (!mouseOverRect(skinsButtonBG, mouseX, mouseY)) {
					highlightRect(&skinsButtonBG, 1 / HLFACTOR);
					hltd = NONE;
				}
				break;
			case OPTIONS:
				if (!mouseOverRect(optionsButtonBG, mouseX, mouseY)) {
					highlightRect(&optionsButtonBG, 1 / HLFACTOR);
					hltd = NONE;
				}
				break;
			case QUIT:
				if (!mouseOverRect(quitButtonBG, mouseX, mouseY)) {
					highlightRect(&quitButtonBG, 1 / HLFACTOR);
					hltd = NONE;
				}
			default:
				break;
			}

			if (mouseOverRect(quitButtonBG, mouseX, mouseY) && leftClick)
				quit = 1;

			SDL_RenderClear(rend);
			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, title, NULL, &titleRect);
			SDL_RenderCopy(rend, buttonBG, NULL, &playButtonBG);
			SDL_RenderCopy(rend, buttonBG, NULL, &skinsButtonBG);
			SDL_RenderCopy(rend, buttonBG, NULL, &optionsButtonBG);
			SDL_RenderCopy(rend, playText, NULL, &playTextRect);
			SDL_RenderCopy(rend, skinsText, NULL, &skinsTextRect);
			SDL_RenderCopy(rend, optionsText, NULL, &optionsTextRect);
			SDL_RenderCopy(rend, buttonBG, NULL, &quitButtonBG);
			SDL_RenderCopy(rend, quitText, NULL, &quitTextRect);
		
			SDL_RenderPresent(rend);
		default:
			break;
		}

		SDL_Delay(1000 / 60);
	}

	killGame(assets);

	return 0;
}
