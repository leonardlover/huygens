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

	SDL_Texture *gameTitle = loadTexture("media/img/gameTitle.png", rend);

	if (!gameTitle)
		return raiseError("Texture", assets);

	if (associateTexture(gameTitle, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect gameTitleRect;

	if (loadRect(gameTitle, &gameTitleRect) != 0)
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

	/* Load skins title */

	SDL_Texture *skinsTitle = loadTexture("media/img/skinsTitle.png", rend);

	if (!skinsTitle)
		return raiseError("Texture", assets);

	if (associateTexture(skinsTitle, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect skinsTitleRect;

	if(loadRect(skinsTitle, &skinsTitleRect) != 0)
		return raiseError("Query", assets);

	/* Load frame */

	SDL_Texture *frame = loadTexture("media/img/frame.png", rend);

	if (!frame)
		return raiseError("Texture", assets);

	if (associateTexture(frame, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect frameRect;

	if(loadRect(frame, &frameRect) != 0)
		return raiseError("Query", assets);
	
	/* Load left arrow */

	SDL_Texture *leftArrow = loadTexture("media/img/leftArrow.png", rend);

	if (!leftArrow)
		return raiseError("Texture", assets);

	if (associateTexture(leftArrow, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect leftArrowRect;

	if(loadRect(leftArrow, &leftArrowRect) != 0)
		return raiseError("Query", assets);

	/* Load right arrow */

	SDL_Texture *rightArrow = loadTexture("media/img/rightArrow.png", rend);

	if (!rightArrow)
		return raiseError("Texture", assets);

	if (associateTexture(rightArrow, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect rightArrowRect;

	if(loadRect(rightArrow, &rightArrowRect) != 0)
		return raiseError("Query", assets);

	/* Load back arrow */

	SDL_Texture *backArrow = loadTexture("media/img/backArrow.png", rend);

	if (!backArrow)
		return raiseError("Texture", assets);

	if (associateTexture(backArrow, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect backArrowRect;

	if(loadRect(backArrow, &backArrowRect) != 0)
		return raiseError("Query", assets);

	/* Load equip text */

	SDL_Texture *equipText = loadTexture("media/img/equipText.png", rend);

	if (!equipText)
		return raiseError("Texture", assets);

	if (associateTexture(equipText, assets) != 0)
		return raiseError("Memory", assets);

	SDL_Rect equipTextRect;

	if(loadRect(equipText, &equipTextRect) != 0)
		return raiseError("Query", assets);

	SDL_Rect equipButtonBG;

	if (loadRect(buttonBG, &equipButtonBG) != 0)
		return raiseError("Query", assets);

	/* Set rectangles positions and sizes */

	/* Main menu assets */

	scaleRect(&gameTitleRect, 3);
	centerXRect(&gameTitleRect);
	setYRect(&gameTitleRect, gameTitleRect.h / 2);

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

	/* Skins menu assets */

	scaleRect(&skinsTitleRect, 2);
	centerXRect(&skinsTitleRect);
	setYRect(&skinsTitleRect, skinsTitleRect.h / 4);

	scaleRect(&frameRect, 5);
	centerRect(&frameRect);

	scaleRect(&leftArrowRect, 2);
	centerYRect(&leftArrowRect);
	setXPropRect(&leftArrowRect, 0.34);

	scaleRect(&rightArrowRect, 2);
	centerYRect(&rightArrowRect);
	setXPropRect(&rightArrowRect, 0.66);

	scaleRect(&backArrowRect, 2);
	setRect(&backArrowRect, backArrowRect.w / 2, backArrowRect.h / 2);

	scaleRect(&equipButtonBG, 0.75);
	centerRect(&equipButtonBG);
	setYPropRect(&equipButtonBG, 0.9);

	scaleRect(&equipTextRect, 4);
	moveRectToRect(&equipTextRect, &equipButtonBG);

	/* Set up game loop variables WILL BE DEPRECATED */

	int quit = 0;

	enum CurrentMenu menu = MAIN_MENU;
	enum HighlightedButton hltd = NONE;

	int mouseX, mouseY;
	int leftClick = 0;

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

		/* Manage menus */

		switch (menu) {
		case MAIN_MENU:
			/* Highlight button under cursor */

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

			/* Change current menu if button is pressed */

			if (hltd == PLAY && leftClick)
				menu = PLAY_MENU;
			if (hltd == SKINS && leftClick)
				menu = SKINS_MENU;
			if (hltd == OPTIONS && leftClick)
				menu = OPTIONS_MENU;
			if (hltd == QUIT && leftClick)
				quit = 1;

			/* Render menu */

			SDL_RenderClear(rend);

			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, gameTitle, NULL, &gameTitleRect);
			SDL_RenderCopy(rend, buttonBG, NULL, &playButtonBG);
			SDL_RenderCopy(rend, buttonBG, NULL, &skinsButtonBG);
			SDL_RenderCopy(rend, buttonBG, NULL, &optionsButtonBG);
			SDL_RenderCopy(rend, playText, NULL, &playTextRect);
			SDL_RenderCopy(rend, skinsText, NULL, &skinsTextRect);
			SDL_RenderCopy(rend, optionsText, NULL, &optionsTextRect);
			SDL_RenderCopy(rend, buttonBG, NULL, &quitButtonBG);
			SDL_RenderCopy(rend, quitText, NULL, &quitTextRect);
		
			SDL_RenderPresent(rend);
			break;
		case PLAY_MENU:
			break;
		case SKINS_MENU:
			/* Render menu */

			SDL_RenderClear(rend);
			SDL_RenderCopy(rend, bg, NULL, NULL);
			SDL_RenderCopy(rend, skinsTitle, NULL, &skinsTitleRect);
			SDL_RenderCopy(rend, frame, NULL, &frameRect);
			SDL_RenderCopy(rend, leftArrow, NULL, &leftArrowRect);
			SDL_RenderCopy(rend, rightArrow, NULL, &rightArrowRect);
			SDL_RenderCopy(rend, backArrow, NULL, &backArrowRect);
			SDL_RenderCopy(rend, buttonBG, NULL, &equipButtonBG);
			SDL_RenderCopy(rend, equipText, NULL, &equipTextRect);
			SDL_RenderPresent(rend);

			break;
		case OPTIONS_MENU:
		default:
			break;
		}

		SDL_Delay(1000 / 60);
	}

	killGame(assets);

	return 0;
}
