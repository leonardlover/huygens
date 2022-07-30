#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "include/utils.c"
#include "include/rect_utils.c"
#include "include/menu_utils.c"

int main(void)
{
	/* Error handling flag */
	enum ErrorType *error;

	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		*error = INITIALIZATION;	
		printError(error);
		return 1;
	}

	/***************************************************/
	/* WHEN GAME IS DONE DECLARE *MOST* VARIABLES HERE */
	/***************************************************/

	/* Create game assets, a manager for game data */
	struct GameAssets *assets = createGameAssets("Huygens", error);

	if (!assets)
		return raiseError(error, assets);

	/*LOAD MAIN MENU ASSETS*/
	if (!loadMainMenuAssets(assets, error))
		return raiseError(error, assets);


	/*SDL_Texture *skinsTitle = _loadTexture("media/img/skinsTitle.png", assets, error);

	if (!skinsTitle)
		return raiseError(error, assets);

	SDL_Rect skinsTitleRect;

	if(loadRect(skinsTitle, &skinsTitleRect) != 0)
		return raiseError(error, assets);


	SDL_Texture *frame = _loadTexture("media/img/frame.png", assets, error);

	if (!frame)
		return raiseError(error, assets);

	SDL_Rect frameRect;

	if(loadRect(frame, &frameRect) != 0)
		return raiseError(error, assets);
	

	SDL_Texture *leftArrow = _loadTexture("media/img/leftArrow.png", assets, error);

	if (!leftArrow)
		return raiseError(error, assets);

	SDL_Rect leftArrowRect;

	if(loadRect(leftArrow, &leftArrowRect) != 0)
		return raiseError(error, assets);


	SDL_Texture *rightArrow = _loadTexture("media/img/rightArrow.png", assets, error);

	if (!rightArrow)
		return raiseError(error, assets);

	SDL_Rect rightArrowRect;

	if(loadRect(rightArrow, &rightArrowRect) != 0)
		return raiseError(error, assets);


	SDL_Texture *equipText = _loadTexture("media/img/equipText.png", assets, error);

	if (!equipText)
		return raiseError(error, assets);

	SDL_Rect equipTextRect;

	if(loadRect(equipText, &equipTextRect) != 0)
		return raiseError(error, assets);

	SDL_Rect equipButtonBG;

	if (loadRect(buttonBG, &equipButtonBG) != 0)
		return raiseError(error, assets);


	SDL_Texture *backArrow = _loadTexture("media/img/backArrow.png", assets, error);

	if (!backArrow)
		return raiseError(error, assets);

	SDL_Rect backArrowRect;

	if(loadRect(backArrow, &backArrowRect) != 0)
		return raiseError(error, assets);*/

	/* Set rectangles positions and sizes */

	/* Main menu assets */


	/* Skins menu assets */

	/*scaleRect(&skinsTitleRect, 2);
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

	scaleRect(&equipButtonBG, 0.75);
	centerRect(&equipButtonBG);
	setYPropRect(&equipButtonBG, 0.9);

	moveRectToRect(&equipTextRect, &equipButtonBG);

	scaleRect(&backArrowRect, 2);
	setRect(&backArrowRect, backArrowRect.w / 2, backArrowRect.h / 2);*/

	/* Set up game loop variables WILL BE DEPRECATED */

	int quit = 0;

	enum Menu menu = MAIN_MENU;
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

			/*switch (hltd) {
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
			}*/

			/* Change current menu if button is pressed */

			/*if (hltd == PLAY && leftClick)
				menu = PLAY_MENU;
			if (hltd == SKINS && leftClick)
				menu = SKINS_MENU;
			if (hltd == OPTIONS && leftClick)
				menu = OPTIONS_MENU;
			if (hltd == QUIT && leftClick)
				quit = 1;*/

			/* Render menu */
			renderAssetData(assets);

			/*SDL_RenderClear(assets->renderer);

			SDL_RenderCopy(assets->renderer, bg, NULL, NULL);
			SDL_RenderCopy(assets->renderer, gameTitle, NULL, &gameTitleRect);
			SDL_RenderCopy(assets->renderer, buttonBG, NULL, &playButtonBG);
			SDL_RenderCopy(assets->renderer, buttonBG, NULL, &skinsButtonBG);
			SDL_RenderCopy(assets->renderer, buttonBG, NULL, &optionsButtonBG);
			SDL_RenderCopy(assets->renderer, playText, NULL, &playTextRect);
			SDL_RenderCopy(assets->renderer, skinsText, NULL, &skinsTextRect);
			SDL_RenderCopy(assets->renderer, optionsText, NULL, &optionsTextRect);
			SDL_RenderCopy(assets->renderer, buttonBG, NULL, &quitButtonBG);
			SDL_RenderCopy(assets->renderer, quitText, NULL, &quitTextRect);
		
			SDL_RenderPresent(assets->renderer);*/
			break;
		case PLAY_MENU:
			break;
		case SKINS_MENU:
			/* Render menu */

			/*if (mouseOverRect(backArrowRect, mouseX, mouseY) && leftClick)
				menu = MAIN_MENU;

			SDL_RenderClear(assets->renderer);
			SDL_RenderCopy(assets->renderer, bg, NULL, NULL);
			SDL_RenderCopy(assets->renderer, skinsTitle, NULL, &skinsTitleRect);
			SDL_RenderCopy(assets->renderer, frame, NULL, &frameRect);
			SDL_RenderCopy(assets->renderer, leftArrow, NULL, &leftArrowRect);
			SDL_RenderCopy(assets->renderer, rightArrow, NULL, &rightArrowRect);
			SDL_RenderCopy(assets->renderer, buttonBG, NULL, &equipButtonBG);
			SDL_RenderCopy(assets->renderer, equipText, NULL, &equipTextRect);
			SDL_RenderCopy(assets->renderer, backArrow, NULL, &backArrowRect);
			SDL_RenderPresent(assets->renderer);*/

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
