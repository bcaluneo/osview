// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include "windows.h"
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "data.hh"
#include "graph.hh"
#include "util.hh"

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED,
														          SDL_WINDOWPOS_CENTERED,
																			SCREEN_WIDTH,
																			SCREEN_HEIGHT,
																			SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	SDL_Surface *icon = IMG_Load("tex/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	SDL_Surface *surface = IMG_Load("tex/bg.png");
	SDL_Texture *tex = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("tex/bar.png");
	SDL_Texture *tex2 = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
	SDL_RenderClear(render);

	std::vector<Graph> graphs {
		{"CPU Graph", 3, 0, {
			{0, 128, 255}, // User color
			{220, 0, 0},   // System color
			{0, 220, 0}    // Idle color
		}},

		{"Memory Usage Graph", 2, 1, {
			{0, 220, 0},  // Inuse color
			{0, 128, 255} // Free color
		}}
	};

	SDL_CreateThread(getData, "Data Thread", static_cast<void*>(graphs.data()));

	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
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
							for (auto &g : graphs) { g.toggleVertical(); }
							break;
					}

					break;
				}
		}

		SDL_Rect bg, bar;
		bg.x = 0;
		bg.y = 0;
		bg.w = SCREEN_WIDTH;
		bg.h = SCREEN_HEIGHT;

		bar.x = BAR_X - 1;
		bar.y = BAR_Y - 1;
		bar.w = BAR_WIDTH + 2;
		bar.h = BAR_HEIGHT + 2 + 10;
		SDL_RenderCopy(render, tex, NULL, &bg);
		SDL_RenderCopy(render, tex2, NULL, &bar);

		bar.y = BAR_Y*BAR_SCALE;
		SDL_RenderCopy(render, tex2, NULL, &bar);

		for (auto g : graphs) {
			g.draw(render);
		}

		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	SDL_Quit();
	return 0;
}
