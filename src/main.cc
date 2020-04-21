#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "data.hh"
#include "graph.hh"
#include <vector>
#include <tuple>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 240;
const int BAR_X = 15;
const int BAR_Y = 40;
const int BAR_SCALE = 4;
const int BAR_WIDTH = SCREEN_WIDTH - (BAR_X*2);
const int BAR_HEIGHT = 50;
const int POLL_TIME = 500;
const int BAND_WIDTH = 5;

bool quit = 0;

SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;
std::vector<std::tuple<double, double, double>> bands;

template <typename T>
void redraw(double data[4], bool graph, std::vector<T> &bands) {
	SDL_Rect usrRect, kerRect, idlRect, memRect, memBgRect;

	memBgRect.x = BAR_X;
	memBgRect.y = BAR_Y*BAR_SCALE;
	memBgRect.w = BAR_WIDTH;
	memBgRect.h = BAR_HEIGHT;
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_RenderFillRect(render, &memBgRect);

	if (graph) {

	} else {
		memRect.x = BAR_X;
		memRect.y = BAR_Y*BAR_SCALE;
		memRect.w = BAR_WIDTH * (data[3] / 100);
		memRect.h = BAR_HEIGHT;
	}

	SDL_SetRenderDrawColor(render, 0, 148, 255, 255);
	SDL_RenderFillRect(render, &memRect);

	if (graph) {
		for (auto it = bands.begin(); it != bands.end(); it++) {
			auto pos = it-bands.begin();
			auto t = *it;

			double usrData = std::get<2>(t);
			double kerData = std::get<1>(t);
			double idlData = std::get<0>(t);

			usrRect.x = BAR_X + pos*BAND_WIDTH;
			kerRect.x = BAR_X + pos*BAND_WIDTH;
			idlRect.x = BAR_X + pos*BAND_WIDTH;
			usrRect.w = kerRect.w = idlRect.w = BAND_WIDTH;
			usrRect.h = BAR_HEIGHT * (usrData/100);
			kerRect.h = BAR_HEIGHT * (kerData/100);
			idlRect.h = BAR_HEIGHT * (idlData/100);
			usrRect.y = BAR_Y + BAR_HEIGHT - usrRect.h;
			kerRect.y = usrRect.y - kerRect.h;
			idlRect.y = kerRect.y - idlRect.h;

			SDL_SetRenderDrawColor(render, 0, 148, 255, 255);
			SDL_RenderFillRect(render, &usrRect);
			SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
			SDL_RenderFillRect(render, &kerRect);
			SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
			SDL_RenderFillRect(render, &idlRect);
		}
	} else {
		usrRect.x = BAR_X;
		usrRect.w = BAR_WIDTH * (data[2] / 100);
		usrRect.y = BAR_Y;
		usrRect.h = BAR_HEIGHT;

		kerRect.x = BAR_X + usrRect.w;
		kerRect.y = BAR_Y;
		kerRect.w = BAR_WIDTH * (data[1] / 100);
		kerRect.h = BAR_HEIGHT;

		idlRect.x = kerRect.x + kerRect.w;
		idlRect.y = BAR_Y;
		idlRect.w = BAR_WIDTH * (data[0] / 100);
		idlRect.h = BAR_HEIGHT;

		SDL_SetRenderDrawColor(render, 0, 148, 255, 255);
		SDL_RenderFillRect(render, &usrRect);
		SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
		SDL_RenderFillRect(render, &kerRect);
		SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
		SDL_RenderFillRect(render, &idlRect);
	}
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
	surface = IMG_Load("tex/bar.png");
	SDL_Texture *tex2 = SDL_CreateTextureFromSurface(render, surface);
	SDL_FreeSurface(surface);

	SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
	SDL_RenderClear(render);

	int colors[3][3] = {
		{0, 128, 255},
		{255, 0, 0},
		{0, 255, 0}
	};

	int memColors[2][3] = {
		{0, 128, 255},
		{0, 255, 0}
	};

	Graph cpuGraph(3, 0);
	cpuGraph.setColors(colors);

	Graph memGraph(2, 1);
	memGraph.setColors(memColors);

	Graph graphs[2] = {cpuGraph, memGraph};

	SDL_CreateThread(getData, "Data Thread", static_cast<void*>(graphs));

	SDL_Event event;
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
						cpuGraph.toggleVertical();
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

		bar.x = BAR_X-1;
		bar.y = BAR_Y-1;
		bar.w = BAR_WIDTH + 2;
		bar.h = BAR_HEIGHT + 2 + 10;
		SDL_RenderCopy(render, tex, NULL, &bg);
		SDL_RenderCopy(render, tex2, NULL, &bar);

		bar.y = BAR_Y*BAR_SCALE;
		SDL_RenderCopy(render, tex2, NULL, &bar);

		// redraw(data, graph, bands);
		cpuGraph.draw();
		memGraph.draw();

		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	printf("\n");
	SDL_Quit();
	return 0;
}
