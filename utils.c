struct GameAssets {
	int isInit;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture **textures;
	size_t texCount;
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

int mouseOverRect(SDL_Rect rect, int x, int y)
{
	if (rect.x <= x && x <= rect.x + rect.w &&
	    rect.y <= y && y <= rect.y + rect.h)
			return 1;

	return 0;
}

void highlightRect(SDL_Rect *rect, float factor)
{
	int w, h, deltaW, deltaH;

	w = rect->w;
	h = rect->h;

	rect->w *= factor;
	rect->h *= factor;

	deltaW = rect->w - w;
	deltaH = rect->h - h;

	rect->x -= deltaW / 2;
	rect->y -= deltaH / 2;
}
