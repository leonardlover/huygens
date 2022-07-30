#ifndef RECT_UTILS_H
#define RECT_UTILS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


void scaleXRect(SDL_Rect *rect, float factor);


void scaleYRect(SDL_Rect *rect, float factor);


void scaleRect(SDL_Rect *rect, float factor);


void setXRect(SDL_Rect *rect, int x);


void setYRect(SDL_Rect *rect, int y);


void setRect(SDL_Rect *rect, int x, int y);


void setXPropRect(SDL_Rect *rect, float prop);


void setYPropRect(SDL_Rect *rect, float prop);


void centerXRect(SDL_Rect *rect);


void centerYRect(SDL_Rect *rect);


void centerRect(SDL_Rect *rect);


void shiftXRect(SDL_Rect *rect, int x);


void shiftYRect(SDL_Rect *rect, int y);


void shiftRect(SDL_Rect *rect, int x, int y);


void moveRectToRect(SDL_Rect *src, SDL_Rect *tgt);


int mouseOverRect(SDL_Rect rect, int x, int y);


void _highlightRect(SDL_Rect *rect, float factor);


void doHighlightRect(SDL_Rect *rect);


void undoHighlightRect(SDL_Rect *rect);


#endif
