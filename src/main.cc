// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include "windows.h"

#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include "NFont.h"
#include <cmath>
#include <utility>
#include <atomic>

#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_ttf.h"
#include "data.hh"
#include "graph.hh"
#include "util.hh"

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

size_t screenWidth = 960;
std::atomic_int screenHeight;
std::atomic_int coreCount;

std::vector<Graph> graphs;

int renderScreen(void *data) {
	SDL_Surface *surface = IMG_Load("tex/bg.png");
	SDL_Texture *tex = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);
	surface = IMG_Load("tex/bar.png");
	SDL_Texture *barTexture = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);

	NFont font(render, "resources/consola.ttf", 23);

	auto totalWidth = (BAR_WIDTH*COLUMN_COUNT) + (HORIZONTAL_SPACING*(COLUMN_COUNT - 1));
	auto cx = (screenWidth - totalWidth) / 2;

	while (!quit) {
		SDL_Rect bg {0, 0, screenWidth, screenHeight.load()};
		SDL_SetRenderDrawColor(render, 60, 60, 60, 255);
	    SDL_RenderFillRect(render, &bg);

		for (signed i = 0; i < graphs.size(); i += 2) {
			std::vector<Graph> columns;
			columns.push_back(graphs[i]);
			if (i != graphs.size() - 1) columns.push_back(graphs[i + 1]);

			for (signed j = 0; j < columns.size(); ++j) {
				Graph g = columns[j];
				signed barX = cx + ((j%2)*(BAR_WIDTH+HORIZONTAL_SPACING));
				signed barY = BAR_Y + (i*VERTICAL_SPACING);
				SDL_Point pos {barX, barY};
				g.draw(pos, barTexture, font, render);
			}
		}

		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	return 0;
}

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	plog::init(plog::debug, "osview.log");
	PLOG_INFO << "OSVIEW is starting.";

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	coreCount.store(sysInfo.dwNumberOfProcessors);
	for (unsigned i = 0; i < coreCount.load(); ++i) {
		std::string graphTitle = "CPU" + std::to_string(i) + " Usage";
		Graph g {graphTitle, {
			{ "usr", ColorTuple {0, 128, 255} }, // User color
			{ "sys", ColorTuple {220, 0, 0} },   // System color
			{ "idl", ColorTuple {0, 220, 0} }    // Idle color
		}};

		graphs.push_back(g);
	}

	Graph memGraph 	{
		"Memory Usage", {
			{ "inuse", ColorTuple {0, 128, 255} },  // Inuse color
			{ "free", ColorTuple {0, 220, 0} } // Free color
	}};
	graphs.push_back(memGraph);
	
	PLOG_INFO << "Discovered " << coreCount.load() << " total processors.";
	PLOG_INFO << "Rendering " << graphs.size() << " graphs.";

	screenHeight.store((BAR_HEIGHT+VERTICAL_SPACING)*std::ceil(((graphs.size() + 1) / 2)));

	window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED,
														          SDL_WINDOWPOS_CENTERED,
																			screenWidth,
																			screenHeight.load(),
																			SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_ENABLE);

	SDL_Surface *icon = IMG_Load("tex/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_FreeSurface(icon);

	SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
	SDL_RenderClear(render);

	SDL_CreateThread(getData, "Data Thread", nullptr);
	SDL_CreateThread(renderScreen, "Render Thread", nullptr);

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
		
		SDL_Delay(1000/60);
	}

	PLOG_INFO << "OSVIEW is stopping.";
	SDL_Quit();
	return 0;
}
