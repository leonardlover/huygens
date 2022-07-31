#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#include "utils.h"
#include "asset_utils.h"
#include "menu_utils.h"

#define WIDTH 1280
#define HEIGHT 720
#define FPS 60
#define DELAY 1000 / FPS

int main(void)
{
	/* Set up random seed */
	srand(time(NULL));

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

	/* Set up play menu variables */
	enum Skin selectedSkin = SHEEP;

	/* Set up skins menu variables */
	struct Asset *skinsMenuBG = NULL;
	struct Asset *skinsTitle = NULL;
	struct Asset *frame = NULL;
	struct Asset *leftArrow = NULL;
	struct Asset *rightArrow = NULL;
	struct Asset *equipButtonBG = NULL;
	struct Asset *equipText = NULL;
	struct Asset *backArrow = NULL;

	char *skinPath = NULL;
	SDL_Texture *skinTexture = NULL;
	SDL_Rect skinRect;

	int loadedSkin = 0;
	enum Skin skin;

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

				scaleAsset(gameTitle, 3);
				centerXAsset(gameTitle);
				setYAsset(gameTitle, (gameTitle->rect)->h / 2);

				scaleAsset(playButtonBG, 0.75);
				setXPropAsset(playButtonBG, 0.2);
				setYPropAsset(playButtonBG, 0.6);
				
				scaleAsset(skinsButtonBG, 0.75);
				setXPropAsset(skinsButtonBG, 0.8);
				setYPropAsset(skinsButtonBG, 0.6);

				scaleAsset(optionsButtonBG, 0.75);
				setXPropAsset(optionsButtonBG, 0.2);
				setYPropAsset(optionsButtonBG, 0.8);

				scaleAsset(quitButtonBG, 0.75);
				setXPropAsset(quitButtonBG, 0.8);
				setYPropAsset(quitButtonBG, 0.8);

				moveAssetToAsset(playText, playButtonBG);
				moveAssetToAsset(skinsText, skinsButtonBG);
				moveAssetToAsset(optionsText, optionsButtonBG);
				moveAssetToAsset(quitText, quitButtonBG);

				selected = NONE;
				highlighted = NONE;

				loaded = 1;
			}

			/* Determine selected button */
			if (mouseOverAsset(playButtonBG, mouseX, mouseY)) {
				selected = PLAY;
			} else if (mouseOverAsset(skinsButtonBG, mouseX, mouseY)) {
				selected = SKINS;
			} else if (mouseOverAsset(optionsButtonBG, mouseX, mouseY)) {
				selected = OPTIONS;
			} else if (mouseOverAsset(quitButtonBG, mouseX, mouseY)) {
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
					undoHighlightAsset(playButtonBG);
					break;
				case SKINS:
					undoHighlightAsset(skinsButtonBG);
					break;
				case OPTIONS:
					undoHighlightAsset(optionsButtonBG);
					break;
				case QUIT:
					undoHighlightAsset(quitButtonBG);
					break;
				default:
					break;
				}

				/* Highlight selected button */
				switch (selected) {
				case PLAY:
					doHighlightAsset(playButtonBG);
					highlighted = PLAY;
					break;
				case SKINS:
					doHighlightAsset(skinsButtonBG);
					highlighted = SKINS;
					break;
				case OPTIONS:
					doHighlightAsset(optionsButtonBG);
					highlighted = OPTIONS;
					break;
				case QUIT:
					doHighlightAsset(quitButtonBG);
					highlighted = QUIT;
					break;
				default:
					break;
				}
			}

			/* Render menu */
			renderAssetData(assets, 1);

			/* Change current menu if button is pressed */
			switch (selected) {
			case PLAY:
				if (enter || (mouseOverAsset(playButtonBG, mouseX, mouseY) && leftClick)) {
					menu = PLAY_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case SKINS:
				if (enter || (mouseOverAsset(skinsButtonBG, mouseX, mouseY) && leftClick)) {
					menu = SKINS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case OPTIONS:
				if (enter || (mouseOverAsset(optionsButtonBG, mouseX, mouseY) && leftClick)) {
					menu = OPTIONS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			case QUIT:
				if (enter || (mouseOverAsset(quitButtonBG, mouseX, mouseY) && leftClick))
					quit = 1;
				break;
			default:
				break;
			}

			break;
		case PLAY_MENU:
			break;
		case SKINS_MENU:
			/* Load assets if not loaded */
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

				scaleAsset(skinsTitle, 2);
				centerXAsset(skinsTitle);
				setYAsset(skinsTitle, (skinsTitle->rect)->h / 4);

				scaleAsset(frame, 5);
				centerAsset(frame);

				scaleAsset(leftArrow, 2);
				centerYAsset(leftArrow);
				setXPropAsset(leftArrow, 0.34);

				scaleAsset(rightArrow, 2);
				centerYAsset(rightArrow);
				setXPropAsset(rightArrow, 0.66);

				scaleAsset(equipButtonBG, 0.75);
				centerAsset(equipButtonBG);
				setYPropAsset(equipButtonBG, 0.9);

				moveAssetToAsset(equipText, equipButtonBG);

				scaleAsset(backArrow, 2);
				setAsset(backArrow, (backArrow->rect)->w / 2, (backArrow->rect)->h / 2);

				selected = NONE;
				highlighted = NONE;

				skin = selectedSkin;

				loaded = 1;
			}

			/* Load skin if not loaded */
			if (!loadedSkin) {
				switch (skin) {
				case SHEEP:
					skinPath = "media/img/skins/sheep.png";
					break;
				case BALLOON:
					skinPath = "media/img/skins/balloon.png";
					break;
				case BUBBLE:
					skinPath = "media/img/skins/bubble.png";
					break;
				case CLOUD:
					skinPath = "media/img/skins/cloud.png";
					break;
				case FLY:
					skinPath = "media/img/skins/fly.png";
					break;
				case MATRIX:
					skinPath = "media/img/skins/matrix.png";
					break;
				case NYAN:
					skinPath = "media/img/skins/nyan.png";
					break;
				case SUPER:
					skinPath = "media/img/skins/super.png";
					break;
				case TAILS:
					skinPath = "media/img/skins/tails.png";
					break;
				default:
					break;
				}

				skinTexture = _loadTexture(skinPath, assets, error);

				if (!skinTexture) {
					SDL_DestroyTexture(skinTexture);
					return raiseError(error, assets);
				}

				if (SDL_QueryTexture(skinTexture, NULL, NULL, &skinRect.w, &skinRect.h) != 0) {
					SDL_DestroyTexture(skinTexture);
					*error = MEMORY;
					return raiseError(error, assets);
				}

				skinRect.w *= 4;
				skinRect.h *= 4;
				skinRect.x = (WIDTH - skinRect.w) / 2;
				skinRect.y = (HEIGHT - skinRect.h) / 2;

				loadedSkin = 1;
			}

			/* Highlight button under cursor */
			if (mouseOverAsset(equipButtonBG, mouseX, mouseY))
				selected = EQUIP;
			else
				selected = NONE;

			if (highlighted != selected) {
				if (highlighted == EQUIP) {
					undoHighlightAsset(equipButtonBG);
					highlighted = NONE;
				}

				if (selected == EQUIP) {
					doHighlightAsset(equipButtonBG);
					highlighted = EQUIP;
				}
			}

			if (enter || (mouseOverAsset(equipButtonBG, mouseX, mouseY) && leftClick))
				selectedSkin = skin;

			/* Change skin if arrow is pressed */
			if (right || (mouseOverAsset(rightArrow, mouseX, mouseY) && leftClick)) {
				skin = (skin + 1) % 9;
				SDL_DestroyTexture(skinTexture);
				loadedSkin = 0;
			}
			if (left || (mouseOverAsset(leftArrow, mouseX, mouseY) && leftClick)) {
				if (skin == SHEEP)
					skin = TAILS;
				else
					skin = (skin - 1) % 9;

				SDL_DestroyTexture(skinTexture);
				loadedSkin = 0;
			}

			/* Render menu */
			renderAssetData(assets, 0);

			/* Render skin */
			SDL_RenderCopy(assets->renderer, skinTexture, NULL, &skinRect);
			SDL_RenderPresent(assets->renderer);

			/* Go back to main menu if back arrow is pressed */
			if (mouseOverAsset(backArrow, mouseX, mouseY) && leftClick) {
				menu = MAIN_MENU;
				SDL_DestroyTexture(skinTexture);
				loadedSkin = 0;
				clearAssetData(assets);
				loaded = 0;
			}
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

	if(skinTexture)
		SDL_DestroyTexture(skinTexture);

	killGame(assets);

	return 0;
}
