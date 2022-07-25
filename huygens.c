#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>
//#include <SDL2/SDL_mixer.h>

#define WIDTH 640
#define HEIGHT 480

#define SPEED 300

int main(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Initialization error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Huygens",
					   SDL_WINDOWPOS_CENTERED,
					   SDL_WINDOWPOS_CENTERED,
					   WIDTH,
					   HEIGHT,
					   0);

	if (!win) {
		printf("Window error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	int render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

	if (!rend) {
		printf("Renderer error: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Surface *surf = IMG_Load("media/sprites/heart.png");

	if (!surf) {
		printf("Surface error: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);

	if (!tex) {
		printf("Texture error: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Rect pos;

	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);

	pos.w *= 8;
	pos.h *= 8;

	float x_pos = (WIDTH - pos.w) / 2;
	float y_pos = (HEIGHT - pos.h) / 2;
	
	float x_vel;
	float y_vel;

	int up;
	int left;
	int down;
	int right;

	int quit = 0;

	while (!quit) {
		SDL_Event event;

		up = 0;
		left = 0;
		down = 0;
		right = 0;

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
					up = 1;
					break;
				case SDL_SCANCODE_A:
					left = 1;
					break;
				case SDL_SCANCODE_S:
					down = 1;
					break;
				case SDL_SCANCODE_D:
					right = 1;
				}
			default:
				break;
			}
		}

		x_vel = 0;
		y_vel = 0;

		if (up)
			y_vel = -SPEED;
		if (left)
			x_vel = -SPEED;
		if (down)
			y_vel = SPEED;
		if (right)
			x_vel = SPEED;

		x_pos += x_vel / 60;
		y_pos += y_vel / 60;

		if (x_pos <= 0)
			x_pos = 0;
		if (y_pos <= 0)
			y_pos = 0;
		if (x_pos >= WIDTH - pos.w)
			x_pos = WIDTH - pos.w;
		if (y_pos >= HEIGHT - pos.h)
			y_pos = HEIGHT - pos.h;

		pos.x = (int) x_pos;
		pos.y = (int) y_pos;

		SDL_RenderClear(rend);

		SDL_RenderCopy(rend, tex, NULL, &pos);
		SDL_RenderPresent(rend);

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
