#define _WIN32_WINNT 0x0501
#include <Windows.h>

#include "SDL.h"
#include "SDL_thread.h"
#include <cstdio>
#include <string>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Threads need to see this.
static bool quit = 0;

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL,
};

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
   static unsigned long long _previousTotalTicks = 0;
   static unsigned long long _previousIdleTicks = 0;

   unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
   unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;

   float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);

   _previousTotalTicks = totalTicks;
   _previousIdleTicks  = idleTicks;
   return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft) {return (((unsigned long long)(ft.dwHighDateTime))<<32)|((unsigned long long)ft.dwLowDateTime);}

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.  Returns -1.0 on error.
float GetCPULoad()
{
   FILETIME idleTime, kernelTime, userTime;
   return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime)+FileTimeToInt64(userTime)) : -1.0f;
}

bool init();
bool loadMedia();
void close();
SDL_Surface *loadSurface(std::string);

SDL_Window *window = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *current = NULL;
SDL_Surface *surfaces[KEY_PRESS_SURFACE_TOTAL];

int printCPULoad(void *data)
{
	while (!quit) {
		float load = GetCPULoad()*100;
		std::cout << load << "\n";
		SDL_Delay(250);
	}

	return 0;
}

int main( int argc, char* args[] )
{
	if(!init())
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		if(!loadMedia())
		{
			printf( "Failed to load media. \n");
		}
		else
		{
			current = surfaces[KEY_PRESS_SURFACE_DEFAULT];
      SDL_BlitSurface(current, NULL, gScreenSurface, NULL);

      SDL_Event event;

			SDL_Thread *printThread = SDL_CreateThread(printCPULoad, "Print Thread", (void *)NULL);

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
								case SDLK_UP:
									current = surfaces[KEY_PRESS_SURFACE_UP];
									break;
								case SDLK_DOWN:
									current = surfaces[KEY_PRESS_SURFACE_DOWN];
									break;
								case SDLK_LEFT:
									current = surfaces[KEY_PRESS_SURFACE_LEFT];
									break;
								case SDLK_RIGHT:
									current = surfaces[KEY_PRESS_SURFACE_RIGHT];
									break;
              }

              break;
          }
        }

				SDL_BlitSurface(current, NULL, gScreenSurface, NULL);
        SDL_UpdateWindowSurface(window);

				SDL_Delay(5);
      }

			SDL_WaitThread(printThread, NULL);
		}
	}

  close();

  return 0;
}

bool init() {
  bool result = 1;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("error init(): sdl failed to initialize with error %s\n", SDL_GetError());
    result = 0;
  } else {
    window = SDL_CreateWindow("osview", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
      printf("error init(): window could nto be created with error %s\n", SDL_GetError());
      result = 0;
    } else {
      gScreenSurface = SDL_GetWindowSurface(window);
    }
  }

  return result;
}

bool loadMedia() {
  bool result = 1;

	surfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("bitmaps/def.bmp");
  if (surfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
    printf("error loadMedia(): unable to load image with error %s\n", SDL_GetError());
    result = 0;
  }

	surfaces[KEY_PRESS_SURFACE_UP] = loadSurface("bitmaps/up.bmp");
	if (surfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		printf("error loadMedia(): unable to load image with error %s\n", SDL_GetError());
		result = 0;
	}

	surfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("bitmaps/down.bmp");
	if (surfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		printf("error loadMedia(): unable to load image with error %s\n", SDL_GetError());
		result = 0;
	}

	surfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("bitmaps/left.bmp");
	if (surfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		printf("error loadMedia(): unable to load image with error %s\n", SDL_GetError());
		result = 0;
	}

	surfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("bitmaps/right.bmp");
	if (surfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		printf("error loadMedia(): unable to load image with error %s\n", SDL_GetError());
		result = 0;
	}

  return result;
}

SDL_Surface *loadSurface(std::string path) {
		SDL_Surface *result = SDL_LoadBMP(path.c_str());
		if (result == NULL) {
			printf("error loadSurface(std::string): unable to load image with error %s\n", SDL_GetError());
		}

		return result;
}

void close() {
	for (auto i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		SDL_FreeSurface(surfaces[i]);
	}

  SDL_Quit();
}
