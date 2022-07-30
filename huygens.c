#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "utils.h"
#include "rect_utils.h"
#include "menu_utils.h"

#define FPS 60
#define DELAY 1000 / FPS

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

	/* Create game assets, a manager for game data */
	struct GameAssets *assets = createGameAssets("Huygens", error);
	if (!assets)
		return raiseError(error, assets);

	/* Set up game loop variables */
	int quit = 0;
	int loaded = 0;
	int mouseX, mouseY;
	int leftClick = 0;
	int up, down, left, right, enter;

	int begin;
	int elapsed;

	enum Menu menu = MAIN_MENU;
	enum Button selected;
	enum Button highlighted;

	SDL_Event event;

	/* Set up main menu variables */
	struct Asset *mainMenuBG = NULL;
	struct Asset *gameTitle = NULL;
	struct Asset *playButtonBG = NULL;
	struct Asset *skinsButtonBG = NULL;
	struct Asset *optionsButtonBG = NULL;
	struct Asset *quitButtonBG = NULL;
	struct Asset *playText = NULL;
	struct Asset *skinsText = NULL;
	struct Asset *optionsText = NULL;
	struct Asset *quitText = NULL;

	/* Set up skins menu variables */
	struct Asset *skinsMenuBG = NULL;
	struct Asset *skinsTitle = NULL;
	struct Asset *frame = NULL;
	struct Asset *leftArrow = NULL;
	struct Asset *rightArrow = NULL;
	struct Asset *equipButtonBG = NULL;
	struct Asset *equipText = NULL;
	struct Asset *backArrow = NULL;

	while (!quit) {
		/* Save time when loop began so it can be considered when doing SDL_Delay */
		begin = SDL_GetTicks();

		/* Reset input flags */
		up = 0;
		down = 0;
		left = 0;
		right = 0;
		enter = 0;
		leftClick = 0;

		/* Manage input */
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftClick = 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_UP:
					up = 1;
					break;
				case SDL_SCANCODE_DOWN:
					down = 1;
					break;
				case SDL_SCANCODE_LEFT:
					left = 1;
					break;
				case SDL_SCANCODE_RIGHT:
					right = 1;
					break;
				case SDL_SCANCODE_RETURN:
					enter = 1;
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		SDL_GetMouseState(&mouseX, &mouseY);

		/* Manage menus */
		switch (menu) {
		case MAIN_MENU:
			/* Load assets if not loaded */
			if (!loaded) {
				mainMenuBG = loadAsset("media/img/bg.png", assets, error);
				if (!mainMenuBG)
					return raiseError(error, assets);

				gameTitle = loadAsset("media/img/gameTitle.png", assets, error);
				if (!gameTitle)
					return raiseError(error, assets);

				playButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!playButtonBG)
					return raiseError(error, assets);

				skinsButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!skinsButtonBG)
					return raiseError(error, assets);

				optionsButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!optionsButtonBG)
					return raiseError(error, assets);

				quitButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!quitButtonBG)
					return raiseError(error, assets);

				playText = loadAsset("media/img/playText.png", assets, error);
				if (!playText)
					return raiseError(error, assets);

				skinsText = loadAsset("media/img/skinsText.png", assets, error);
				if (!skinsText)
					return raiseError(error, assets);

				optionsText = loadAsset("media/img/optionsText.png", assets, error);
				if (!optionsText)
					return raiseError(error, assets);

				quitText = loadAsset("media/img/quitText.png", assets, error);
				if (!quitText)
					return raiseError(error, assets);

				scaleRect(gameTitle->rect, 3);
				centerXRect(gameTitle->rect);
				setYRect(gameTitle->rect, (gameTitle->rect)->h / 2);

				scaleRect(playButtonBG->rect, 0.75);
				setXPropRect(playButtonBG->rect, 0.2);
				setYPropRect(playButtonBG->rect, 0.6);
				
				scaleRect(skinsButtonBG->rect, 0.75);
				setXPropRect(skinsButtonBG->rect, 0.8);
				setYPropRect(skinsButtonBG->rect, 0.6);

				scaleRect(optionsButtonBG->rect, 0.75);
				setXPropRect(optionsButtonBG->rect, 0.2);
				setYPropRect(optionsButtonBG->rect, 0.8);

				scaleRect(quitButtonBG->rect, 0.75);
				setXPropRect(quitButtonBG->rect, 0.8);
				setYPropRect(quitButtonBG->rect, 0.8);

				moveRectToRect(playText->rect, playButtonBG->rect);
				moveRectToRect(skinsText->rect, skinsButtonBG->rect);
				moveRectToRect(optionsText->rect, optionsButtonBG->rect);
				moveRectToRect(quitText->rect, quitButtonBG->rect);

				selected = NONE;
				highlighted = NONE;

				loaded = 1;
			}

			/* Determine selected button */
			if (mouseOverRect(playButtonBG->rect, mouseX, mouseY)) {
				selected = PLAY;
			} else if (mouseOverRect(skinsButtonBG->rect, mouseX, mouseY)) {
				selected = SKINS;
			} else if (mouseOverRect(optionsButtonBG->rect, mouseX, mouseY)) {
				selected = OPTIONS;
			} else if (mouseOverRect(quitButtonBG->rect, mouseX, mouseY)) {
				selected = QUIT;
			} else if (up || down) {
				switch (selected) {
				case PLAY:
					selected = OPTIONS;
					break;
				case SKINS:
					selected = QUIT;
					break;
				case OPTIONS:
				case NONE:
					selected = PLAY;
					break;
				case QUIT:
					selected = SKINS;
					break;
				default:
					break;
				}
			} else if (left || right) {
				switch (selected) {
				case PLAY:
					selected = SKINS;
					break;
				case SKINS:
				case NONE:
					selected = PLAY;
					break;
				case OPTIONS:
					selected = QUIT;
					break;
				case QUIT:
					selected = OPTIONS;
					break;
				default:
					break;
				}
			}

			/* Highlight selected button if not already highlighted */
			if (highlighted != selected) {
				/* Remove current highlight */
				switch (highlighted) {
				case PLAY:
					undoHighlightRect(playButtonBG->rect);
					break;
				case SKINS:
					undoHighlightRect(skinsButtonBG->rect);
					break;
				case OPTIONS:
					undoHighlightRect(optionsButtonBG->rect);
					break;
				case QUIT:
					undoHighlightRect(quitButtonBG->rect);
					break;
				default:
					break;
				}

				/* Highlight selected button */
				switch (selected) {
				case PLAY:
					doHighlightRect(playButtonBG->rect);
					highlighted = PLAY;
					break;
				case SKINS:
					doHighlightRect(skinsButtonBG->rect);
					highlighted = SKINS;
					break;
				case OPTIONS:
					doHighlightRect(optionsButtonBG->rect);
					highlighted = OPTIONS;
					break;
				case QUIT:
					doHighlightRect(quitButtonBG->rect);
					highlighted = QUIT;
					break;
				default:
					break;
				}
			}

			/* Change current menu if button is pressed */
			switch (selected) {
			case PLAY:
				if (enter || (mouseOverRect(playButtonBG->rect, mouseX, mouseY) && leftClick)) {
					menu = PLAY_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case SKINS:
				if (enter || (mouseOverRect(skinsButtonBG->rect, mouseX, mouseY) && leftClick)) {
					menu = SKINS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case OPTIONS:
				if (enter || (mouseOverRect(optionsButtonBG->rect, mouseX, mouseY) && leftClick)) {
					menu = OPTIONS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case QUIT:
				if (enter || (mouseOverRect(quitButtonBG->rect, mouseX, mouseY) && leftClick))
					quit = 1;
				break;
			default:
				break;
			}

			/* Render menu */
			renderAssetData(assets);
			break;
		case PLAY_MENU:
			break;
		case SKINS_MENU:
			if (!loaded) {
				skinsMenuBG = loadAsset("media/img/bg.png", assets, error);
				if (!skinsMenuBG)
					return raiseError(error, assets);

				skinsTitle = loadAsset("media/img/skinsTitle.png", assets, error);
				if (!skinsTitle)
					return raiseError(error, assets);

				frame = loadAsset("media/img/frame.png", assets, error);
				if (!frame)
					return raiseError(error, assets);

				leftArrow = loadAsset("media/img/leftArrow.png", assets, error);
				if (!leftArrow)
					return raiseError(error, assets);

				rightArrow = loadAsset("media/img/rightArrow.png", assets, error);
				if (!rightArrow)
					return raiseError(error, assets);

				equipButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!equipButtonBG)
					return raiseError(error, assets);

				equipText = loadAsset("media/img/equipText.png", assets, error);
				if (!equipText)
					return raiseError(error, assets);

				backArrow = loadAsset("media/img/backArrow.png", assets, error);
				if (!backArrow)
					return raiseError(error, assets);

				scaleRect(skinsTitle->rect, 2);
				centerXRect(skinsTitle->rect);
				setYRect(skinsTitle->rect, (skinsTitle->rect)->h / 4);

				scaleRect(frame->rect, 5);
				centerRect(frame->rect);

				scaleRect(leftArrow->rect, 2);
				centerYRect(leftArrow->rect);
				setXPropRect(leftArrow->rect, 0.34);

				scaleRect(rightArrow->rect, 2);
				centerYRect(rightArrow->rect);
				setXPropRect(rightArrow->rect, 0.66);

				scaleRect(equipButtonBG->rect, 0.75);
				centerRect(equipButtonBG->rect);
				setYPropRect(equipButtonBG->rect, 0.9);

				moveRectToRect(equipText->rect, equipButtonBG->rect);

				scaleRect(backArrow->rect, 2);
				setRect(backArrow->rect, (backArrow->rect)->w / 2, (backArrow->rect)->h / 2);

				selected = NONE;
				highlighted = NONE;

				loaded = 1;
			}

			/* Highlight button under cursor */
			if (mouseOverRect(equipButtonBG->rect, mouseX, mouseY))
				selected = EQUIP;
			else
				selected = NONE;

			if (highlighted != selected) {
				if (highlighted == EQUIP) {
					undoHighlightRect(equipButtonBG->rect);
					highlighted = NONE;
				}

				if (selected == EQUIP) {
					doHighlightRect(equipButtonBG->rect);
					highlighted = EQUIP;
				}
			}

			/* Go back to main menu if vack arrow is pressed */
			if (mouseOverRect(backArrow->rect, mouseX, mouseY) && leftClick) {
				menu = MAIN_MENU;
				clearAssetData(assets);
				loaded = 0;
			}

			/* Render menu */
			renderAssetData(assets);
			break;
		case OPTIONS_MENU:
			break;
		default:
			break;
		}

		/* Make sure that the max refresh rate is DELAY */
		elapsed = SDL_GetTicks() - begin;
		if (elapsed < DELAY)
			SDL_Delay(DELAY - elapsed);
	}

	killGame(assets);

	return 0;
}
