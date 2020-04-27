// Copyright (C) Brendan Caluneo

#include <vector>
#include <tuple>
#include <memory>
#include "SDL.h"

#ifndef GRAPH_HH
#define GRAPH_HH

extern SDL_Renderer *render;
extern const size_t SCREEN_WIDTH;
extern const size_t BAR_X;
extern const size_t BAR_Y;
extern const size_t BAR_SCALE;
extern const size_t BAR_WIDTH;
extern const size_t BAR_HEIGHT;
extern const size_t BAND_WIDTH;
extern const size_t TOTAL_BANDS;

class Graph {
  public:
    Graph(size_t dataCount, size_t scale);
    ~Graph();

    void draw();
    void updateSize(int index, double amount);
    void insertBand(double *band);
    void setColors(int colors[][3]);
    void toggleVertical();

  private:
    bool vertical = 1;
    size_t currBandPos = 0;
    size_t dataCount = 0;
    size_t scale = 0;

    std::shared_ptr<std::vector<SDL_Rect>> rects;
    std::shared_ptr<std::vector<int>> colors;

    SDL_Rect **vRects;
    double **bands;
};

#endif
