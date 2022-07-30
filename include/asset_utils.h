#ifndef RECT_UTILS_H
#define RECT_UTILS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "utils.h"


void scaleXAsset(struct Asset *asset, float factor);


void scaleYAsset(struct Asset *asset, float factor);


void scaleAsset(struct Asset *asset, float factor);


void setXAsset(struct Asset *asset, int x);


void setYAsset(struct Asset *asset, int y);


void setAsset(struct Asset *asset, int x, int y);


void setXPropAsset(struct Asset *asset, float prop);


void setYPropAsset(struct Asset *asset, float prop);


void centerXAsset(struct Asset *asset);


void centerYAsset(struct Asset *asset);


void centerAsset(struct Asset *asset);


void shiftXAsset(struct Asset *asset, int x);


void shiftYAsset(struct Asset *asset, int y);


void shiftAsset(struct Asset *asset, int x, int y);


void moveAssetToAsset(struct Asset *src, struct Asset *tgt);


int mouseOverAsset(struct Asset *asset, int x, int y);


void _highlightAsset(struct Asset *asset, float factor);


void doHighlightAsset(struct Asset *asset);


void undoHighlightAsset(struct Asset *asset);


#endif
