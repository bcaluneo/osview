#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "time.hh"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 120;
const int BAR_X = 15;
const int BAR_Y = 40;
const int BAR_WIDTH = SCREEN_WIDTH - (BAR_X*2);
const int BAR_HEIGHT = 50;
const int POLL_TIME = 200;

bool quit = 0;

SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;

void redraw(double data[3], bool graph) {
	SDL_Rect bar;
	bar.x = BAR_X-1;
	bar.y = BAR_Y-1;
	bar.w = BAR_WIDTH + 2;
	bar.h = BAR_HEIGHT + 2;

	SDL_Rect usrRect;
	usrRect.x = BAR_X;
	usrRect.w = BAR_WIDTH * (data[2] / 100);
	usrRect.y = BAR_Y;
	usrRect.h = BAR_HEIGHT;

	SDL_Rect kerRect;
	kerRect.x = BAR_X + usrRect.w;
	kerRect.y = BAR_Y;
	kerRect.w = BAR_WIDTH * (data[1] / 100);
	kerRect.h = BAR_HEIGHT;

	SDL_Rect idlRect;
	idlRect.x = kerRect.x + kerRect.w;
	idlRect.y = BAR_Y;
	idlRect.w = BAR_WIDTH * (data[0] / 100);
	idlRect.h = BAR_HEIGHT;

	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderFillRect(render, &bar);
	SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
	SDL_RenderFillRect(render, &usrRect);
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render, &kerRect);
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_RenderFillRect(render, &idlRect);
}

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED,
														SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
														SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_Surface *icon = IMG_Load("tex/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	SDL_Surface *surface = IMG_Load("tex/bg.png");
	SDL_Texture *tex = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
	SDL_RenderClear(render);
	double data[3] = {0.0, 0.0, 0.0};
	SDL_CreateThread(getData, "Print Thread", (void *) data);
	SDL_Event event;
	bool graph = 0;
	while (!quit) {
		if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = 1;
						break;
					case SDLK_h:
						graph = 1 - graph;
						break;
				}

				break;
			}
		}

		SDL_Rect bg;
		bg.x = 0;
		bg.y = 0;
		bg.w = SCREEN_WIDTH;
		bg.h = SCREEN_HEIGHT;
		SDL_RenderCopy(render, tex, NULL, &bg);

		redraw(data, graph);
		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	printf("\n");
	SDL_Quit();
	return 0;
}
