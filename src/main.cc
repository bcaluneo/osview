#define _WIN32_WINNT 0x0501
#include <Windows.h>

#include "SDL.h"
#include "SDL_thread.h"
#include <cstdio>
#include <cstdint>
#include <string>
#include <iostream>

const int SCREEN_WIDTH = 780;
const int SCREEN_HEIGHT = 120;
const int BAR_X = 15;
const int BAR_Y = 25;
const int BAR_WIDTH = SCREEN_WIDTH - (BAR_X*2);
const int BAR_HEIGHT = 65;
const int POLL_TIME = 500;

static bool quit = 0;

SDL_Window *window = nullptr;
SDL_Renderer *render = nullptr;

void GetFileTime(FILETIME &result, const FILETIME &start, const FILETIME &end) {
	ULARGE_INTEGER largeStart = { 0 };
	largeStart.LowPart = start.dwLowDateTime;
	largeStart.HighPart = start.dwHighDateTime;

	ULARGE_INTEGER largeEnd = { 0 };
	largeEnd.LowPart = end.dwLowDateTime;
	largeEnd.HighPart = end.dwHighDateTime;

	largeStart.QuadPart = largeEnd.QuadPart - largeStart.QuadPart;

	result.dwLowDateTime = largeStart.LowPart;
	result.dwHighDateTime = largeStart.HighPart;
}

double percent(double a, double b) {
	return a*100 / b;
}

int getData(void *data)
{
	printf("CPU:\tIdl:\tKer:\tUsr:\n");

	double *ret = static_cast<double*>(data);

	while (!quit) {
		FILETIME a0, a1, a2, b0, b1, b2;
		GetSystemTimes(&a0, &a1, &a2);
		SDL_Delay(POLL_TIME);
		GetSystemTimes(&b0, &b1, &b2);

		FILETIME finalIdle, finalKernel, finalUser;

		GetFileTime(finalIdle, a0, b0);
		GetFileTime(finalKernel, a1, b1);
		GetFileTime(finalUser, a2, b2);

		double idleTime, kernelTime, userTime;

		idleTime = finalIdle.dwLowDateTime;
		kernelTime = finalKernel.dwLowDateTime;
		userTime = finalUser.dwLowDateTime;

		double total = (kernelTime+userTime); // ker + idl + usr

		// idle time is removed because the cpu can't be utilized if it's idle.
		// kernel time includes idle time so it's still included in the total time
		// because we're looking at the utilization time over the total time.
		// this number is different from what appears in task manager
		// but it's similar to the cpu usage in speedfan.
		// this leads me to believe that task manager only shows %usr in the
		// performance tab and speedfun looks at the usage over a period of a 1000
		// using the cpu time similar to what i've done.
		double cpu = (kernelTime+userTime-idleTime)*100 / total;

		ret[0] = percent(idleTime, total);
		ret[1] = percent(kernelTime - idleTime, total);
		ret[2] = percent(userTime, total);

		printf("%.2f\t%.2f\t%.2f\t%.2f\r", cpu, percent(idleTime, total), percent(kernelTime-idleTime, total), percent(userTime, total));
	}

	return 0;
}

void redraw(double data[3]) {
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

	SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
	SDL_RenderFillRect(render, &usrRect);
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render, &kerRect);
	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	SDL_RenderFillRect(render, &idlRect);
}

int main(int argc, char **args) {
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(render, 180, 180, 180, 255);
	SDL_RenderClear(render);

	double data[3] = {0.0, 0.0, 0.0};
	SDL_CreateThread(getData, "Print Thread", (void *) data);

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
          }

          break;
      }
    }

		redraw(data);
		SDL_RenderPresent(render);
		SDL_Delay(20);
  }

	printf("\n");
  SDL_Quit();

  return 0;
}
