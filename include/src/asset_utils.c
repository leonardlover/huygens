#ifndef RECT_UTILS_C
#define RECT_UTILS_C


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "asset_utils.h"


#ifndef WIDTH
#define WIDTH 1280
#endif


#ifndef HEIGHT
#define HEIGHT 720
#endif


#ifndef HLFACTOR
#define HLFACTOR 1.1
#endif


void scaleXAsset(struct Asset *asset, float factor)
{
	(asset->rect)->w *= factor;
}


void scaleYAsset(struct Asset *asset, float factor)
{
	(asset->rect)->h *= factor;
}


void scaleAsset(struct Asset *asset, float factor)
{
	scaleXAsset(asset, factor);
	scaleYAsset(asset, factor);
}


void setXAsset(struct Asset *asset, int x)
{
	(asset->rect)->x = x;
}


void setYAsset(struct Asset *asset, int y)
{
	(asset->rect)->y = y;
}


void setAsset(struct Asset *asset, int x, int y)
{
	setXAsset(asset, x);
	setYAsset(asset, y);
}


void setXPropAsset(struct Asset *asset, float prop)
{
	setXAsset(asset, (WIDTH - (asset->rect)->w) * prop);
}


void setYPropAsset(struct Asset *asset, float prop)
{
	setYAsset(asset, (HEIGHT - (asset->rect)->h) * prop);
}


void centerXAsset(struct Asset *asset)
{
	setXPropAsset(asset, 0.5);
}


void centerYAsset(struct Asset *asset)
{
	setYPropAsset(asset, 0.5);
}


void centerAsset(struct Asset *asset)
{
	centerXAsset(asset);
	centerYAsset(asset);
}


void shiftXAsset(struct Asset *asset, int x)
{
	setXAsset(asset, (asset->rect)->x + x);
}


void shiftYAsset(struct Asset *asset, int y)
{
	setYAsset(asset, (asset->rect)->y + y);
}


void shiftAsset(struct Asset *asset, int x, int y)
{
	shiftXAsset(asset, x);
	shiftYAsset(asset, y);
}


void moveAssetToAsset(struct Asset *src, struct Asset *tgt)
{
	setAsset(src, (tgt->rect)->x, (tgt->rect)->y);
	shiftAsset(src, ((tgt->rect)->w - (src->rect)->w) / 2, ((tgt->rect)->h - (src->rect)->h) / 2);
}


int mouseOverAsset(struct Asset *asset, int x, int y)
{
	if ((asset->rect)->x <= x && x <= (asset->rect)->x + (asset->rect)->w &&
	    (asset->rect)->y <= y && y <= (asset->rect)->y + (asset->rect)->h)
			return 1;

	return 0;
}


void _highlightAsset(struct Asset *asset, float factor)
{
	int w, h;

	w = (asset->rect)->w;
	h = (asset->rect)->h;

	scaleAsset(asset, factor);

	shiftAsset(asset, (w - (asset->rect)->w) / 2, (h - (asset->rect)->h) / 2);
}


void doHighlightAsset(struct Asset *asset)
{
	_highlightAsset(asset, HLFACTOR);
}


void undoHighlightAsset(struct Asset *asset)
{
	_highlightAsset(asset, 1 / HLFACTOR);
}


#endif
