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

#define WIDTH 1260
#define HEIGHT 700
#define FPS 60
#define DELAY 1000 / FPS

int puntos(int points){
	switch(points%10){
		case 0:	return 1;
		case 1:	return 43;
		case 2:	return 85;
		case 3:	return 127;
		case 4:	return 169;
		case 5:	return 211;
		case 6:	return 253;
		case 7:	return 295;
		case 8:	return 337;
		case 9:	return 379;
	}
}

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
	struct GameAssets *assets = createGameAssets("Go! Sheep Dog!", error);
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
	/*struct Asset *optionsButtonBG = NULL;*/
	struct Asset *quitButtonBG = NULL;
	struct Asset *playText = NULL;
	struct Asset *skinsText = NULL;
	/*struct Asset *optionsText = NULL;*/
	struct Asset *quitText = NULL;

	/* Set up play menu variables */
	enum Skin selectedSkin = SHEEP;
	const unsigned char *key; 
	key = SDL_GetKeyboardState(NULL);

	/* Set up skins menu variables */
	struct Asset *skinsMenuBG = NULL;
	struct Asset *skinsTitle = NULL;
	struct Asset *frame = NULL;
	struct Asset *leftArrow = NULL;
	struct Asset *rightArrow = NULL;
	struct Asset *equipButtonBG = NULL;
	struct Asset *equipText = NULL;
	struct Asset *backArrow = NULL;

	char *skinPath = "media/img/sheep.png";
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
				mainMenuBG = loadAsset("media/img/Background.png", assets, error);
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

				/*optionsButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!optionsButtonBG)
					return raiseError(error, assets);*/

				quitButtonBG = loadAsset("media/img/buttonBG.png", assets, error);
				if (!quitButtonBG)
					return raiseError(error, assets);

				playText = loadAsset("media/img/playText.png", assets, error);
				if (!playText)
					return raiseError(error, assets);

				skinsText = loadAsset("media/img/skinsText.png", assets, error);
				if (!skinsText)
					return raiseError(error, assets);

				/*optionsText = loadAsset("media/img/optionsText.png", assets, error);
				if (!optionsText)
					return raiseError(error, assets);*/

				quitText = loadAsset("media/img/quitText.png", assets, error);
				if (!quitText)
					return raiseError(error, assets);

				scaleAsset(gameTitle, 5);
				centerXAsset(gameTitle);
				setYAsset(gameTitle, (gameTitle->rect)->h / 2);

				scaleAsset(playButtonBG, 0.75);
				setXPropAsset(playButtonBG, 0.2);
				setYPropAsset(playButtonBG, 0.7);
				
				scaleAsset(skinsButtonBG, 0.75);
				setXPropAsset(skinsButtonBG, 0.8);
				setYPropAsset(skinsButtonBG, 0.7);

				/*scaleAsset(optionsButtonBG, 0.75);
				setXPropAsset(optionsButtonBG, 0.2);
				setYPropAsset(optionsButtonBG, 0.8);*/

				scaleAsset(quitButtonBG, 0.75);
				centerXAsset(quitButtonBG);
				setYPropAsset(quitButtonBG, 0.9);

				moveAssetToAsset(playText, playButtonBG);
				moveAssetToAsset(skinsText, skinsButtonBG);
				/*moveAssetToAsset(optionsText, optionsButtonBG);*/
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
			} /*else if (mouseOverAsset(optionsButtonBG, mouseX, mouseY)) {
				selected = OPTIONS;
			} */else if (mouseOverAsset(quitButtonBG, mouseX, mouseY)) {
				selected = QUIT;
			} else if (up || down) {
				switch (selected) {
				case PLAY:
					selected = QUIT;
					break;
				case SKINS:
					selected = QUIT;
					break;
				/*case OPTIONS:*/
				case NONE:
					selected = PLAY;
					break;
				case QUIT:
					selected = PLAY;
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
				/*case OPTIONS:
					selected = QUIT;
					break;*/
				case QUIT:
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
				/*case OPTIONS:
					undoHighlightAsset(optionsButtonBG);
					break;*/
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
				/*case OPTIONS:
					doHighlightAsset(optionsButtonBG);
					highlighted = OPTIONS;
					break;*/
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
					quit = 1;
				}
				break;
			case SKINS:
				if (enter || (mouseOverAsset(skinsButtonBG, mouseX, mouseY) && leftClick)) {
					menu = SKINS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;
			/*case OPTIONS:
				if (enter || (mouseOverAsset(optionsButtonBG, mouseX, mouseY) && leftClick)) {
					menu = OPTIONS_MENU;
					clearAssetData(assets);
					loaded = 0;
				}
				break;*/
			case QUIT:
				if (enter || (mouseOverAsset(quitButtonBG, mouseX, mouseY) && leftClick))
					quit = 1;
				break;
			default:
				break;
			}

			break;
		/*case PLAY_MENU:
			break;*/
		case SKINS_MENU:
			/* Load assets if not loaded */
			if (!loaded) {
				skinsMenuBG = loadAsset("media/img/Background.png", assets, error);
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
				/*case CLOUD:
					skinPath = "media/img/skins/cloud.png";
					break;*/
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
				skin = (skin + 1) % 8;
				SDL_DestroyTexture(skinTexture);
				loadedSkin = 0;
			}
			if (left || (mouseOverAsset(leftArrow, mouseX, mouseY) && leftClick)) {
				if (skin == SHEEP)
					skin = TAILS;
				else
					skin = (skin - 1) % 8;

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
		/*case OPTIONS_MENU:
			break;*/
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

	if (menu == PLAY_MENU) {
		int quitGame = 0;
		
		switch (selectedSkin) {
		case SHEEP:
			skinPath = "media/img/skins/spriteSheets/sheep.png";
			break;
		case BALLOON:
			skinPath = "media/img/skins/spriteSheets/balloon.png";
			break;
		case BUBBLE:
			skinPath = "media/img/skins/spriteSheets/bubble.png";
			break;
		/*case CLOUD:
			skinPath = "media/img/skins/spriteSheets/cloud.png";
			break;*/
		case FLY:
			skinPath = "media/img/skins/spriteSheets/fly.png";
			break;
		case MATRIX:
			skinPath = "media/img/skins/spriteSheets/matrix.png";
			break;
		case NYAN:
			skinPath = "media/img/skins/spriteSheets/nyan.png";
			break;
		case SUPER:
			skinPath = "media/img/skins/spriteSheets/super.png";
			break;
		case TAILS:
			skinPath = "media/img/skins/spriteSheets/tails.png";
			break;
		default:
			break;
		}

		SDL_Texture *spriteCar = NULL;
		SDL_Surface *temp = IMG_Load(skinPath);
		spriteCar = SDL_CreateTextureFromSurface(assets->renderer, temp);
		SDL_FreeSurface(temp);

		SDL_Rect car; //hitbox
		car.x = 300; 
		car.y = 300;
		car.w = 32;
		car.h = 40;

		SDL_Rect textureRect; 
		textureRect.x = 0;
		textureRect.y = 0;
		textureRect.w = 32;
		textureRect.h = 40;
		SDL_QueryTexture(spriteCar, NULL, NULL, &textureRect.w, &textureRect.h);
			
		SDL_Texture *spriteTube1[3] = {NULL,NULL,NULL};
		SDL_Texture *spriteTube2[3] = {NULL,NULL,NULL};
		SDL_Texture *spriteCrate[3] = {NULL,NULL,NULL};
		SDL_Surface *tempTube2 = IMG_Load("media/img/Box2.png");
		SDL_Surface *tempCrate = IMG_Load("media/img/Crate.png");
		    
		SDL_Rect tuberia1[3]; //hitbox
		SDL_Rect tuberia2[3]; //hitbox
		SDL_Rect crate[3];
			
		for (int i = 0; i < 3; i++) {
			tuberia1[i].x = WIDTH - 70 + ((i+1)*WIDTH)/3;
			tuberia1[i].y = 0;
			tuberia1[i].w = 60; 
			tuberia1[i].h = rand() % (HEIGHT - 425 +1) + 125; 

			tuberia2[i].x = tuberia1[i].x;
			tuberia2[i].y = tuberia1[i].h + 150; 
			tuberia2[i].w = 60; 
			tuberia2[i].h = HEIGHT - tuberia2[i].y;

			crate[i].x = tuberia1[i].x;
			crate[i].y = tuberia1[i].h;
			crate[i].w = 60;
			crate[i].h = 150;
		}

		for (int i = 0; i < 3; i++) {
			spriteTube1[i] = SDL_CreateTextureFromSurface(assets->renderer, tempTube2);
			spriteTube2[i] = SDL_CreateTextureFromSurface(assets->renderer, tempTube2);
			spriteCrate[i] = SDL_CreateTextureFromSurface(assets->renderer, tempCrate);
		}

		//SDL_FreeSurface(tempTube1);
		SDL_FreeSurface(tempTube2);
		SDL_FreeSurface(tempCrate);

		SDL_Rect textureCrate;
		textureCrate.x = 0;
		textureCrate.y = 0;
		
		SDL_Rect textureTube1[3] = {{0,0,60,0},{0,0,60,0},{0,0,60,0}};

		SDL_Rect textureTube2[3] = {{0,0,60,0},{0,0,60,0},{0,0,60,0}};
		
		for (int i = 0; i < 3; i++) {
			if(tuberia1[i].h <= 300) textureTube1[i].h = 210;
			else textureTube1[i].h = 350;
			if(tuberia2[i].h <= 300) textureTube2[i].h = 210;
			else textureTube2[i].h = 350;
			SDL_QueryTexture(spriteTube1[i] , NULL, NULL, &textureTube1[i].w, &textureTube1[i].h);
			SDL_QueryTexture(spriteTube2[i] , NULL, NULL, &textureTube2[i].w, &textureTube2[i].h);
			SDL_QueryTexture(spriteCrate[i] , NULL, NULL, &textureCrate.w, &textureCrate.h);
		}


		SDL_Texture *spriteNum[4] = {NULL,NULL,NULL,NULL};
		SDL_Surface *tempNum = IMG_Load("media/img/SpriteSheetNumbers.png");

		for (int i = 0; i < 4; i++)
    		    spriteNum[i] = SDL_CreateTextureFromSurface(assets->renderer, tempNum);

		SDL_Rect number[4] = {{405,20,40,60},{360,20,40,60},{315,20,40,60},{270,20,40,60}};

		SDL_Rect textureNumber[4] = {{1,1,40,60},{1,1,40,60},{1,1,40,60},{1,1,40,60}};
		
		for (int i = 0; i < 4; i++)
			SDL_QueryTexture(spriteNum[i], NULL, NULL, &textureNumber[i].w,&textureNumber[i].h);

		SDL_Texture *spriteScore = NULL;
		SDL_Surface *tempScore = IMG_Load("media/img/Score.png");
		spriteScore = SDL_CreateTextureFromSurface(assets->renderer, tempScore);
		SDL_FreeSurface(tempScore);

		SDL_Rect score = {10,20,250,60}; //hitbox

		SDL_Rect textureScore; 
		textureScore.x = 0;
		textureScore.y = 0;
		SDL_QueryTexture(spriteScore, NULL, NULL, &textureScore.w, &textureScore.h);

		SDL_Texture *spriteClouds[2] = {NULL,NULL};
		SDL_Surface *tempClouds = IMG_Load("media/img/Clouds.png");
		spriteClouds[0] = SDL_CreateTextureFromSurface(assets->renderer, tempClouds);
		spriteClouds[1] = SDL_CreateTextureFromSurface(assets->renderer, tempClouds);
		SDL_FreeSurface(tempClouds);

		SDL_Rect clouds[2] = {{0,675,WIDTH,25},{WIDTH,675,WIDTH,25}}; //hitbox

		SDL_Rect textureClouds; 
		textureClouds.x = 0;
		textureClouds.y = 0;
		SDL_QueryTexture(spriteClouds[0], NULL, NULL, &textureClouds.w, &textureClouds.h);
		SDL_QueryTexture(spriteClouds[1], NULL, NULL, &textureClouds.w, &textureClouds.h);

		SDL_Texture *spriteBigCloud = NULL;
		SDL_Surface *tempBigCloud = IMG_Load("media/img/BigCloud.png");
		spriteBigCloud = SDL_CreateTextureFromSurface(assets->renderer, tempBigCloud);
		SDL_FreeSurface(tempBigCloud);

		SDL_Rect bigCloud = {0,700,1260,700}; //hitbox

		SDL_Rect texturebigCloud; 
		texturebigCloud.x = 0;
		texturebigCloud.y = 0;
		texturebigCloud.w = 1260;
		texturebigCloud.h = 700;
		SDL_QueryTexture(spriteBigCloud, NULL, NULL, &texturebigCloud.w, &texturebigCloud.h);

		SDL_Texture *spriteText = NULL;
		SDL_Surface *tempText = IMG_Load("media/img/TextoInicio.png");
		spriteText = SDL_CreateTextureFromSurface(assets->renderer, tempText);

		SDL_FreeSurface(tempText);

		SDL_Rect textInicio = {WIDTH/2-325,HEIGHT,650,60}; //hitbox

		SDL_Rect textureText; 
		textureText.x = 0;
		textureText.y = 0;
		SDL_QueryTexture(spriteBigCloud, NULL, NULL, &textureText.w, &textureText.h);

		SDL_Texture *spritePause = NULL;
		SDL_Surface *tempPause = IMG_Load("media/img/Pause.png");
		spritePause = SDL_CreateTextureFromSurface(assets->renderer, tempPause);
		SDL_FreeSurface(tempPause);

		SDL_Rect imgPause = {WIDTH/2 - 105,HEIGHT/2 - 155,210,310}; //hitbox

		SDL_Rect texturePause; 
		texturePause.x = 0;
		texturePause.y = 0;
		texturePause.w = 210;
		texturePause.h = 310;
		SDL_QueryTexture(spritePause, NULL, NULL, &texturePause.w, &texturePause.h);

		SDL_Texture *spriteSelect = NULL;
		SDL_Surface *tempSelect = IMG_Load("media/img/Select.png");
		spriteSelect = SDL_CreateTextureFromSurface(assets->renderer, tempSelect);
		SDL_FreeSurface(tempSelect);

		SDL_Rect select = {WIDTH/2 - 91,HEIGHT,182,45}; //hitbox

		SDL_Rect textureSelect; 
		textureSelect.x = 0;
		textureSelect.y = 0;
		SDL_QueryTexture(spriteSelect, NULL, NULL, &textureSelect.w, &textureSelect.h);

		SDL_Texture *spriteTransicion = NULL;
		SDL_Surface *tempTransicion = IMG_Load("media/img/Transicion.png");
		spriteTransicion = SDL_CreateTextureFromSurface(assets->renderer, tempTransicion);
		SDL_FreeSurface(tempTransicion);

		SDL_Rect transicion = {0,0,WIDTH,HEIGHT}; //hitbox

		SDL_Rect textureTransicion; 
		textureTransicion.x = 0;
		textureTransicion.y = 0;
		textureTransicion.w = 1;
		textureTransicion.h = 1;
		SDL_QueryTexture(spriteTransicion, NULL, NULL, &textureTransicion.w, &textureTransicion.h);

		SDL_Texture *spriteGameOver = NULL;
		SDL_Surface *tempGameOver = IMG_Load("media/img/GameOverScreen.png");
		spriteGameOver = SDL_CreateTextureFromSurface(assets->renderer, tempGameOver);
		SDL_FreeSurface(tempGameOver);

		SDL_Rect gameOver = {0,0,1260,700}; //hitbox

		SDL_Rect textureGameOver; 
		textureGameOver.x = 0;
		textureGameOver.y = 0;
		textureGameOver.w = 1260;
		textureGameOver.h = 700;
		SDL_QueryTexture(spriteGameOver, NULL, NULL, &textureGameOver.w, &textureGameOver.h);


		SDL_Texture *spriteEndCloud = NULL;
		SDL_Surface *tempEndCloud = IMG_Load("media/img/endCloud.png");
		spriteEndCloud = SDL_CreateTextureFromSurface(assets->renderer, tempEndCloud);
		SDL_FreeSurface(tempEndCloud);

		SDL_Rect endCloud = {0,HEIGHT,1260,25}; //hitbox

		SDL_Rect textureEndCloud; 
		textureEndCloud.x = 0;
		textureEndCloud.y = 0;
		textureEndCloud.w = 1260;
		textureEndCloud.h = 25;
		SDL_QueryTexture(spriteEndCloud, NULL, NULL, &textureEndCloud.w, &textureEndCloud.h);

		SDL_Texture *spriteScreenScore = NULL;
		SDL_Surface *tempScreenScore = IMG_Load("media/img/ScoreScreen.png");
		spriteScreenScore = SDL_CreateTextureFromSurface(assets->renderer, tempScreenScore);
		SDL_FreeSurface(tempScreenScore);

		SDL_Rect screenScore = {WIDTH/2 -325,- HEIGHT,650,350}; //hitbox

		SDL_Rect textureScreenScore; 
		textureScreenScore.x = 0;
		textureScreenScore.y = 0;
		SDL_QueryTexture(spriteScreenScore, NULL, NULL, &textureScreenScore.w, &textureScreenScore.h);

		SDL_Texture *background = _loadTexture("media/img/Background.png", assets, error);

		SDL_Texture *campanil = _loadTexture("media/img/CampanilRotatorio.png", assets, error);
		SDL_Rect campRot = {0,0,WIDTH,HEIGHT};
		SDL_Rect textureCampRot = {0,0,WIDTH,HEIGHT};

		float yMove = -2;
		float bigCMove = 0;
		float scoreMove = 10;
		float textMove = 5;
		char xMove = 3;
		unsigned long int count = 0; 
		unsigned int points = 0;
		char stop = 0, space = 0, cloudsMove = 1, campMove = 10;
		char fase = 0, pause = 0, index = 0, endIndex = 0, reset = 2;

		for (int i = 0; i < 4; i++) {
			textureNumber[i].x = 1;
			textureNumber[i].y = 1;
			textureNumber[i].w = 40;
			textureNumber[i].h = 60;
		}

		textureTransicion.x = 0;
		textureTransicion.y = 0;
		textureTransicion.w = 1;
		textureTransicion.h = 1;

		textureRect.x = 0;
		textureRect.y = 0;
		textureRect.w = 32;
		textureRect.h = 40;

		SDL_Texture *sheet = _loadTexture(skinPath, assets, error);
		SDL_Rect shifter;
		SDL_QueryTexture(sheet, NULL, NULL, &shifter.w, &shifter.h);

		switch (selectedSkin) {
		case SHEEP:
			skinPath = "media/img/skins/sheep.png";
			break;
		case BALLOON:
			skinPath = "media/img/skins/balloon.png";
			break;
		case BUBBLE:
			skinPath = "media/img/skins/bubble.png";
			break;
		/*case CLOUD:
			skinPath = "media/img/skins/cloud.png";
			break;*/
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

		SDL_Texture *standaloneSkin = _loadTexture(skinPath, assets, error);
		SDL_QueryTexture(standaloneSkin, NULL, NULL, &textureRect.w, &textureRect.h);

		SDL_Event e; //aqui se guarda el "evento", un click o pulsar una tecla.
		while (!quitGame) {
			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT)
					quitGame  = 1;

				if (e.type == SDL_KEYUP && !stop && !pause && reset == 0) {
					if (!key[SDL_SCANCODE_SPACE]) {
						//printf("%d\n", key[SDLK_SPACE]);
						//printf("Levantar\n");
						space = 0;
					}
				}

				if (e.type == SDL_KEYDOWN && reset == 0) {
					//printf("Se pulso una tecla\n"); //eventos teclado
					if (key[SDL_SCANCODE_SPACE] && space == 0  && !stop && !pause && (textInicio.y == 500 || fase != 0)) {
						//printf("%d\n", key[SDLK_SPACE]);
						yMove = -5;
						space = 1;
						if (fase == 0) {
							fase = 1;
							count = 0;
						}
					}

					if (key[SDL_SCANCODE_RETURN] && pause==0 && (car.y + car.h) <= 675) {
						if (pause == 0 && fase == 1) {
							pause = 1;
							//printf("Pausa\n");
							select.y = imgPause.y + 122;
							if(select.x != WIDTH/2 - 91) select.x = WIDTH/2 - 91;
						}
					} else if(key[SDL_SCANCODE_RETURN] && ((pause==1 && reset == 0 && fase != 2) || (fase == 2 && screenScore.y == (HEIGHT/2 - 175)))) {
						if (fase != 2 && pause == 1) {
							if (index==0) {
								//printf("Despausa\n");
								pause = 0;
							}
							if (index==1) {
								//printf("Reset\n");
								reset = 1;
							}
							if (index==2) {
								pause = 0;
								//printf("Salir\n");
								quitGame = 1;
							}
						} else if (fase == 2 && pause == 0) {
							if (endIndex==0) {
								//printf("Reset End\n");
								reset = 1;
							}
							if (endIndex==1) {
								pause = 0;
								//printf("Salir End\n");
								quitGame = 1;
							}
						}
					}

					if (key[SDL_SCANCODE_UP] && reset == 0) {
						//printf("FLecha arriba presionada.\n");
						index--;
						select.y -= 64;
						//printf("%d\n", index);
					} else if (key[SDL_SCANCODE_DOWN] && reset == 0) {
						//printf("Flecha abajo presionada.\n");
						index++;
						select.y += 64;
						//printf("%d\n", index);
					}

					if (index < 0) {
						index = 2;
						select.y = imgPause.y + 250;
					}

					if (index > 2) {
						index = 0;
						select.y = imgPause.y + 122;
					}

					if (key[SDL_SCANCODE_RIGHT] && fase == 2) {
						//printf("FLecha derecha presionada.\n");
						endIndex++;
						select.x = screenScore.x + 372;
						//printf("%d\n", endIndex);
					} else if (key[SDL_SCANCODE_LEFT] && fase == 2) {
						//printf("FLecha derecha presionada.\n");
						endIndex--;
						select.x = screenScore.x + 96;
						//printf("%d\n", endIndex);
					}

					if (endIndex < 0) {
						endIndex = 1;
						select.x = screenScore.x + 372;
					}
					if (endIndex > 1) {
						endIndex = 0;
						select.x = screenScore.x + 96;
					}
				}
			}

			if (reset == 2) {
				//printf("REPITO!\n");
				scoreMove = 10;
				campMove = 10;
				textMove = 5;
				index = 0;
				textInicio.y = HEIGHT;
				endIndex = 0;
				reset = 0;
				fase = 0;
				textureTransicion.x = 0;
				textureRect.x = 0;
				pause = 0;
				count = 0;
				points = 0;
				stop = 0;
				car.x = 300; //pos inicial
				car.y = 300; 
				car.w = textureRect.w; //ancho y alto
				car.h = textureRect.h;
				yMove = -2;
				xMove = 3;
				space = 0;
				cloudsMove = 1;
				bigCMove = 1;
				clouds[0].x = 0;
				clouds[1].x = WIDTH;
				clouds[0].y = HEIGHT-25;
				clouds[1].y = HEIGHT-25;
				bigCloud.y = HEIGHT;
				endCloud.y = HEIGHT;
				screenScore.y = -HEIGHT;

				for (int i = 0; i < 4; i++) {
					textureNumber[i].x = 1;
					textureNumber[i].y = 1;
					textureNumber[i].w = 40;
					textureNumber[i].h = 60;
				}

				for(int i = 0; i < 3; i++) {
					tuberia1[i].x = WIDTH -70 + ((i+1)*WIDTH)/3;;
					tuberia1[i].y = 0;
					tuberia1[i].w = 60; 
					tuberia1[i].h = rand() % (HEIGHT - 425 + 1) + 125; 

					tuberia2[i].x = WIDTH - 70 + ((i+1)*WIDTH)/3;;
					tuberia2[i].y = tuberia1[i].h + 150; 
					tuberia2[i].w = 60; 
					tuberia2[i].h = HEIGHT - tuberia2[i].y;

					crate[i].x = tuberia1[i].x;
					crate[i].y = tuberia1[i].h;
					crate[i].w = 60;
					crate[i].h = 150;

					if(tuberia1[i].h <= 300) textureTube1[i].h = 210;
					else textureTube1[i].h = 350;
					if(tuberia2[i].h <= 300) textureTube2[i].h = 210;
					else textureTube2[i].h = 350;
				}
				continue;
			}
			
			if (fase != 2 || pause == 1) {
				count++;
			}

			if (count%250 == 0) {  //para que no se mueva constantemente
				if (fase == 0) {
					if (count%1000 == 0) textureRect.x = (textureRect.x + textureRect.w) % shifter.w;
					if ((count/250) % campMove == 0) textureCampRot.x = (textureCampRot.x + WIDTH) % (WIDTH*8);
					clouds[0].x -= cloudsMove;
					clouds[1].x -= cloudsMove;
					car.y += yMove;
					if (car.y < 300) yMove += 0.1;
					else if (car.y > 300) yMove += -0.1;
					for (int i = 0; i < 2; i++) {
						if (clouds[i].x<=-WIDTH) clouds[i].x = clouds[(i+1)%2].x + WIDTH;
					}
					if (textureTransicion.x < 26 && count%750 == 0) textureTransicion.x++;
					else if (textInicio.y > 500 && textureTransicion.x == 26) {
						textInicio.y -= textMove;
						if (textMove > 1.1) textMove -= 0.1;
					}
					else if (textInicio.y < 500) textInicio.y = 500;
				}
				if (!stop && fase == 1 && pause == 0) {
					if (textInicio.y < HEIGHT) {
						textInicio.y += textMove;
						if (textMove < 5) textMove += 0.05;
					}
					if (count%1000 == 0) textureRect.x = (textureRect.x + textureRect.w) % shifter.w;
					if ((count/250) % campMove == 0) textureCampRot.x = (textureCampRot.x + WIDTH) % (WIDTH*8);
					for (int i = 0; i < 3; i++) {
						tuberia1[i].x -= xMove;
						tuberia2[i].x -= xMove;
						crate[i].x -= xMove;
					}
					car.y += yMove;
					yMove += 0.2;
					clouds[0].x -= cloudsMove;
					clouds[1].x -= cloudsMove;

					for (int i = 0; i < 3; i++) {
						if (tuberia1[i].x < -60) {  //vuelve a aparecer por la derecha
							tuberia1[i].x = tuberia1[(i+5)%3].x + WIDTH/3 + tuberia1[i].w;
							tuberia1[i].h = rand() % (HEIGHT - 425 + 1) + 125;
							
							tuberia2[i].x = tuberia1[i].x;
							tuberia2[i].y = tuberia1[i].h + 150;
							tuberia2[i].h = HEIGHT - tuberia2[i].y;

							crate[i].x = tuberia1[i].x;
							crate[i].y = tuberia1[i].h;
						}
						if (tuberia1[i].h <= 300) textureTube1[i].h = 210;
						else textureTube1[i].h = 350;
						if (tuberia2[i].h <= 300) textureTube2[i].h = 210;
						else textureTube2[i].h = 350;
					}

					for (int i = 0; i < 2; i++)
						if(clouds[i].x<=-WIDTH) clouds[i].x = clouds[(i+1)%2].x + WIDTH;

					if (car.x > WIDTH - car.w)
						car.x = WIDTH - car.w;
					if (car.x < 0)
						car.x = 0;
					if (car.y < 0)
						car.y = 0;
					if (car.y > HEIGHT - car.h)
						car.y = HEIGHT - car.h;

					if (car.x >= (tuberia1[points%3].x+tuberia1[points%3].w)) {
						points++;
						//printf("Puntos: %u\n", points);
						if (xMove<10) {
							if (points % 5 == 0 && points <= 10) {
								xMove++;
								cloudsMove++;
							}
							else if (points % 20 == 0 && points > 10) {
								xMove++;
								cloudsMove++;
								campMove--;
							}
						}
					}
					for (int i = 0; i < 4; i++) {
						int newPoint = (points / (int)(pow(10,i))) % 10;
						textureNumber[i].x = puntos(newPoint);
						textureNumber[i].y = 1;
						textureNumber[i].w = 40;
						textureNumber[i].h = 60;
					}
				} else if (stop && !pause) {
					car.x += xMove; 
					car.y += yMove;
					yMove += 0.2;
				}
				if (reset == 1) {
					if (textureTransicion.x > 0) textureTransicion.x--;
					else if (textureTransicion.x == 0)
						reset = 2;
				}

				if (fase == 2) {
					if (endCloud.y + endCloud.h >= 0) {
						texturebigCloud.w = 1260;
						texturebigCloud.h = 700;
						if (bigCMove > 1.1) bigCMove -= 0.065;
						bigCloud.y -= bigCMove;
						endCloud.y = bigCloud.y + bigCloud.h;
					} else if ((bigCloud.y - bigCloud.h < 0) && screenScore.y < (HEIGHT/2 - 175)) {
						screenScore.y += scoreMove;
						if (scoreMove >1.1) scoreMove -= 0.1;
					} else if (screenScore.y >= (HEIGHT/2 - 175)) {
						screenScore.y = (HEIGHT/2 - 175);
						select.y = screenScore.y + 275;
						if (select.x != (screenScore.x + 96) && select.x != (screenScore.x + 372))
							select.x = screenScore.x + 96;
					}
					stop = 1;
				}

				SDL_RenderClear(assets->renderer);
				SDL_RenderCopy(assets->renderer, background, NULL, NULL);
				//SDL_SetRenderDrawColor(assets->renderer, 150,150,255,255);
				
				if (fase != 2) {
					SDL_RenderCopy(assets->renderer, campanil, &textureCampRot,  &campRot);
					for (int i = 0; i < 3; i++) {
						SDL_RenderCopy(assets->renderer, spriteTube1[i], &textureTube1[i], &tuberia1[i]);
						SDL_RenderCopy(assets->renderer, spriteTube2[i], &textureTube2[i], &tuberia2[i]);
						SDL_RenderCopy(assets->renderer, spriteCrate[i], &textureCrate, &crate[i]);
					}

					SDL_RenderCopy(assets->renderer, spriteCar, &textureRect, &car);
					
					for (int i = 0; i < 4; i++)
						SDL_RenderCopy(assets->renderer, spriteNum[i], &textureNumber[i], &number[i]);

					SDL_RenderCopy(assets->renderer, spriteScore, &textureScore, &score);
					SDL_RenderCopy(assets->renderer, spriteText, &textureText, &textInicio);

					if (pause) {
						SDL_RenderCopy(assets->renderer, spritePause, &texturePause, &imgPause);
						SDL_RenderCopy(assets->renderer, spriteSelect, &textureSelect, &select);
					}
				}

				if (fase == 2) {
					SDL_RenderCopy(assets->renderer, spriteGameOver, &textureGameOver, &gameOver);
					SDL_RenderCopy(assets->renderer, spriteScreenScore, &textureScreenScore, &screenScore);
					SDL_RenderCopy(assets->renderer, spriteEndCloud, &textureEndCloud, &endCloud);
					if (screenScore.y == (HEIGHT/2 - 175))
						SDL_RenderCopy(assets->renderer, spriteSelect, &textureSelect, &select);
				}

				SDL_RenderCopy(assets->renderer, spriteBigCloud, &texturebigCloud, &bigCloud);
				SDL_RenderCopy(assets->renderer, spriteClouds[0], &textureClouds, &clouds[0]);
				SDL_RenderCopy(assets->renderer, spriteClouds[1], &textureClouds, &clouds[1]);
				SDL_RenderCopy(assets->renderer, spriteTransicion, &textureTransicion, &transicion);
				SDL_RenderPresent(assets->renderer);

				if (car.y+car.h >= clouds[0].y && fase != 2) {
					//printf("colision\n");
					//printf("Choque con el suelo...\n");
					stop = 1;
					if (bigCloud.y>0) {
						texturebigCloud.w = 1260;
						texturebigCloud.h = 700;
						if (bigCMove < 10) bigCMove+= 0.1;
						bigCloud.y -= bigCMove;
						clouds[0].y -= bigCMove;
						clouds[1].y -= bigCMove;
					}
					if (bigCloud.y <= 0) {
						bigCloud.y = 0;
						fase = 2;
					}
					continue;
				}

				if (stop != 0) continue;
				for (int i = 0; i < 3; i++) {
					if (car.x <= tuberia1[i].x+tuberia1[i].w  && car.x+car.w >= tuberia1[i].x) { //verifica colision horizontal
						if (car.y <= tuberia1[i].y + tuberia1[i].h || car.y+car.h >= tuberia2[i].y) { //verifica colision vertical
							/*printf("colision\n");
							printf("(%d,%d,%d,%d) ",car.x,car.x + car.w,car.y,car.y + car.h);
							printf("- (%d,%d,%d,%d) ",tuberia1[i].x,tuberia1[i].x+tuberia1[i].w,tuberia1[i].y,tuberia1[i].y + tuberia1[i].h);
							printf("- (%d,%d,%d,%d)\n",tuberia2[i].x,tuberia2[i].x+tuberia2[i].w,tuberia2[i].y,tuberia2[i].y + tuberia2[i].h);*/
							stop = 1;
							continue;
						}
					}
				}
			}
		}

		if (background)
			SDL_DestroyTexture(background);

		if (campanil)
			SDL_DestroyTexture(campanil);

		if (sheet)
			SDL_DestroyTexture(sheet);

		if (standaloneSkin)
			SDL_DestroyTexture(standaloneSkin);
	}

	killGame(assets);

	return 0;
}
