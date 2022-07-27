struct GameAssets {
	int isInit;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture **textures;
	size_t texCount;
};

enum CurrentMenu {
	MAIN_MENU,
	PLAY_MENU,
	SKINS_MENU,
	OPTIONS_MENU,
};

enum HighlightedButton {
	NONE,
	PLAY,
	SKINS,
	OPTIONS,
	QUIT,
};

struct GameAssets *createGameAssets(void)
{
	struct GameAssets *assets = calloc(1, sizeof(struct GameAssets));

	return assets;
}

void initAssets(struct GameAssets *assets)
{
	assets->isInit = 1;
}

void associateWindow(SDL_Window *window, struct GameAssets *assets)
{
	assets->window = window;
}

void associateRenderer(SDL_Renderer *renderer, struct GameAssets *assets)
{
	assets->renderer = renderer;
}

int associateTexture(SDL_Texture *tex, struct GameAssets *assets)
{
	if (assets->texCount == 0)
		assets->textures = calloc(1, sizeof(SDL_Texture *));
	else
		assets->textures = realloc(assets->textures, (assets->texCount + 1) * sizeof(SDL_Texture *));
		
	if (!assets->textures)
		return 1;

	assets->textures[assets->texCount] = tex;
	assets->texCount += 1;

	return 0;
}

void killGame(struct GameAssets *assets)
{
	if (assets->textures) {
		for (size_t i = 0; i < assets->texCount; i++)
			SDL_DestroyTexture(assets->textures[i]);

		free(assets->textures);
	}

	if (assets->renderer)
		SDL_DestroyRenderer(assets->renderer);

	if (assets->window)
		SDL_DestroyWindow(assets->window);

	if (assets->isInit) {
		assets->isInit = 0;
		SDL_Quit();
	}

	free(assets);
}

int raiseError(char *prompt, struct GameAssets *assets)
{
	if (!assets) {
		printf("%s error.\n", prompt);
		return 1;
	}

	printf("%s error: %s\n", prompt, SDL_GetError());
	killGame(assets);
	return 1;
}

SDL_Texture *loadTexture(char *path, SDL_Renderer *rend)
{
	SDL_Surface *surf = IMG_Load(path);

	if (!surf)
		return NULL;

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surf);
	SDL_FreeSurface(surf);

	return tex;
}

int loadRect(SDL_Texture *tex, SDL_Rect *rect)
{
	return SDL_QueryTexture(tex, NULL, NULL, &rect->w, &rect->h);
}

void scaleRect(SDL_Rect *rect, float factor)
{
	rect->w *= factor;
	rect->h *= factor;
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

void highlightRect(SDL_Rect *rect, float factor)
{
	int w, h;

	w = rect->w;
	h = rect->h;

	scaleRect(rect, factor);

	shiftRect(rect, (w - rect->w) / 2, (h - rect->h) / 2);
}
