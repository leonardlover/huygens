#ifndef RECT_UTILS_C
#define RECT_UTILS_C


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "rect_utils.h"


#ifndef WIDTH
#define WIDTH 1280
#endif


#ifndef HEIGHT
#define HEIGHT 720
#endif


#ifndef HLFACTOR
#define HLFACTOR 1.1
#endif


void scaleXRect(SDL_Rect *rect, float factor)
{
	rect->w *= factor;
}


void scaleYRect(SDL_Rect *rect, float factor)
{
	rect->h *= factor;
}


void scaleRect(SDL_Rect *rect, float factor)
{
	scaleXRect(rect, factor);
	scaleYRect(rect, factor);
}


void setXRect(SDL_Rect *rect, int x)
{
	rect->x = x;
}


void setYRect(SDL_Rect *rect, int y)
{
	rect->y = y;
}


void setRect(SDL_Rect *rect, int x, int y)
{
	setXRect(rect, x);
	setYRect(rect, y);
}


void setXPropRect(SDL_Rect *rect, float prop)
{
	setXRect(rect, (WIDTH - rect->w) * prop);
}


void setYPropRect(SDL_Rect *rect, float prop)
{
	setYRect(rect, (HEIGHT - rect->h) * prop);
}


void centerXRect(SDL_Rect *rect)
{
	setXPropRect(rect, 0.5);
}


void centerYRect(SDL_Rect *rect)
{
	setYPropRect(rect, 0.5);
}


void centerRect(SDL_Rect *rect)
{
	centerXRect(rect);
	centerYRect(rect);
}


void shiftXRect(SDL_Rect *rect, int x)
{
	setXRect(rect, rect->x + x);
}


void shiftYRect(SDL_Rect *rect, int y)
{
	setYRect(rect, rect->y + y);
}


void shiftRect(SDL_Rect *rect, int x, int y)
{
	shiftXRect(rect, x);
	shiftYRect(rect, y);
}


void moveRectToRect(SDL_Rect *src, SDL_Rect *tgt)
{
	setRect(src, tgt->x, tgt->y);
	shiftRect(src, (tgt->w - src->w) / 2, (tgt->h - src->h) / 2);
}


int mouseOverRect(SDL_Rect rect, int x, int y)
{
	if (rect.x <= x && x <= rect.x + rect.w &&
	    rect.y <= y && y <= rect.y + rect.h)
			return 1;

	return 0;
}


void _highlightRect(SDL_Rect *rect, float factor)
{
	int w, h;

	w = rect->w;
	h = rect->h;

	scaleRect(rect, HLFACTOR);

	shiftRect(rect, (w - rect->w) / 2, (h - rect->h) / 2);
}


void doHighlightRect(SDL_Rect *rect)
{
	_highlightRect(rect, HLFACTOR);
}


void undoHighlightRect(SDL_Rect *rect)
{
	_highlightRect(rect, 1 / HLFACTOR);
}


#endif
