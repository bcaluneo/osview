// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include "windows.h"
#include "sysinfoapi.h"
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_ttf.h"
#include "data.hh"
#include "graph.hh"
#include "util.hh"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include <winternl.h>
#include "NFont.h"

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	plog::init(plog::debug, "osview.log");
	PLOG_INFO << "OSVIEW is starting.";

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
	SDL_Texture *barTexture = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
	SDL_RenderClear(render);

	std::vector<Graph> graphs {
		{"CPU Graph", 3, {BAR_X, BAR_Y}, {
			{0, 128, 255}, // User color
			{220, 0, 0},   // System color
			{0, 220, 0}    // Idle color
		}},

		{"Memory Usage Graph", 2, {BAR_X, 1+(BAR_SCALE*BAR_Y)}, {
			{0, 128, 255},  // Inuse color
			{0, 220, 0} // Free color
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
				case SDL_KEYUP:
					auto k = event.key.keysym.sym;
					if (k == SDLK_ESCAPE) quit = 1;
					if (k == SDLK_h) for (auto &g : graphs) { g.toggleVertical(); };
					if (k == SDLK_a) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
						                                        "About",
																										"osview\nGraphical System Resource Monitoring Tool\nWritten by Brendan Caluneo",
																										window);

					break;
				}
		}

		SDL_Rect bg {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
		SDL_RenderCopy(render, tex, NULL, &bg);

		for (auto g : graphs) {
			g.draw(barTexture, render);
		}

		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	PLOG_INFO << "OSVIEW is stopping.";
	SDL_Quit();
	return 0;
}
