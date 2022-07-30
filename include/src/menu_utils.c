#ifndef MENU_UTILS_C
#define MENU_UTILS_C


#include "utils.h"
#include "rect_utils.h"
#include "menu_utils.h"


#ifndef WIDTH
#define WIDTH 1280
#endif


#ifndef HEIGHT
#define HEIGHT 720
#endif


int loadMainMenuAssets(struct GameAssets *assets, enum ErrorType *error)
{
	struct Asset *bg = loadAsset("media/img/bg.png", assets, error);

	if (!bg)
		return raiseError(error, assets);

	struct Asset *gameTitle = loadAsset("media/img/gameTitle.png", assets, error);

	if (!gameTitle)
		return raiseError(error, assets);

	struct Asset *playButtonBG = loadAsset("media/img/buttonBG.png", assets, error);

	if (!playButtonBG)
		return raiseError(error, assets);

	struct Asset *skinsButtonBG = loadAsset("media/img/buttonBG.png", assets, error);

	if (!skinsButtonBG)
		return raiseError(error, assets);

	struct Asset *optionsButtonBG = loadAsset("media/img/buttonBG.png", assets, error);

	if (!optionsButtonBG)
		return raiseError(error, assets);

	struct Asset *quitButtonBG = loadAsset("media/img/buttonBG.png", assets, error);

	if (!quitButtonBG)
		return raiseError(error, assets);

	struct Asset *playText = loadAsset("media/img/playText.png", assets, error);

	if (!playText)
		return raiseError(error, assets);

	struct Asset *skinsText = loadAsset("media/img/skinsText.png", assets, error);

	if (!skinsText)
		return raiseError(error, assets);

	struct Asset *optionsText = loadAsset("media/img/optionsText.png", assets, error);

	if (!optionsText)
		return raiseError(error, assets);

	struct Asset *quitText = loadAsset("media/img/quitText.png", assets, error);

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
}


#endif
