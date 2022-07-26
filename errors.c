void printError(char *prompt)
{
	printf("%s: %s\n", prompt, SDL_GetError());
}

int initError(void)
{
	printError("Initialization error");
	return 1;
}

int windowError(void)
{
	printError("Window error");
	SDL_Quit();
	return 1;
}

int rendererError(SDL_Window *win)
{
	printError("Renderer error");
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 1;
}

int textureError(SDL_Renderer *rend, SDL_Window *win)
{
	printError("Texture error");
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 1;
}

int queryError(SDL_Texture *tex, SDL_Renderer *rend, SDL_Window *win)
{
	printError("Query error");
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 1;
}

