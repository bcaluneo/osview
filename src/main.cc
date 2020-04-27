// Copyright (C) Brendan Caluneo

#define _WIN32_WINNT 0x0501
#include <Windows.h>
#include <iostream>
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_image.h"
#include "SDL_syswm.h"
#include "data.hh"
#include "graph.hh"
#include "resource.hh"

const size_t MENU_HEIGHT = 20;
const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 260;
const size_t BAR_X = 15;
const size_t BAR_Y = 40;
const size_t BAR_SCALE = 4;
const size_t BAR_WIDTH = SCREEN_WIDTH - (BAR_X*2);
const size_t BAR_HEIGHT = 50;
const size_t POLL_TIME = 200;
const size_t BAND_WIDTH = 5;
const size_t TOTAL_BANDS = BAR_WIDTH / BAND_WIDTH;

bool quit = 0;
SDL_Window *window;
SDL_Renderer *render;

HWND getHWND() {
   SDL_SysWMinfo wndInfo;
	 SDL_VERSION(&wndInfo.version);
   SDL_GetWindowWMInfo(window, &wndInfo);
   return wndInfo.info.win.window;
}

void WndProc(void *userData, void *hwnd, unsigned int message,
	long long unsigned int wParam, long long int lParam)
{
    switch (message)
    {
			case WM_COMMAND:
				switch (LOWORD(wParam)) {
					case ID_EXIT:
						quit = 1;
						break;
				}

				break;
		}
}

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED,
														SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
														SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SetMenu(getHWND(), LoadMenu(NULL, MAKEINTRESOURCE(100)));

	SDL_SetWindowsMessageHook(WndProc, NULL);

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
		{220, 0, 0},
		{0, 220, 0}
	};

	int memColors[2][3] = {
		{0, 220, 0},
		{0, 128, 255}
	};

	Graph cpuGraph(3, 0);
	cpuGraph.setColors(colors);

	Graph memGraph(2, 1);
	memGraph.setColors(memColors);

	Graph graphs[2] = { cpuGraph, memGraph };

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
						memGraph.toggleVertical();
						break;
				}

				break;
			}
		}

		SDL_Rect bg, bar;
		bg.x = 0;
		bg.y = 0;
		bg.w = SCREEN_WIDTH;
		bg.h = SCREEN_HEIGHT - MENU_HEIGHT;

		bar.x = BAR_X-1;
		bar.y = BAR_Y-1;
		bar.w = BAR_WIDTH + 2;
		bar.h = BAR_HEIGHT + 2 + 10;
		SDL_RenderCopy(render, tex, NULL, &bg);
		SDL_RenderCopy(render, tex2, NULL, &bar);

		bar.y = BAR_Y*BAR_SCALE;
		SDL_RenderCopy(render, tex2, NULL, &bar);

		cpuGraph.draw();
		memGraph.draw();

		SDL_RenderPresent(render);
		SDL_Delay(1000/60);
	}

	SDL_Quit();
	std::cout << std::endl;
	return 0;
}
