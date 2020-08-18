// Copyright (C) Brendan Caluneo

#include <vector>
#include <memory>
#include "SDL.h"
#include "util.hh"

#ifndef GRAPH_HH
#define GRAPH_HH

extern SDL_Renderer *render;

class Graph {
  public:
    Graph(size_t dataCount, size_t scale, ColorArray colors);
    ~Graph();

    void draw();
    void updateSize(int index, double amount);
    void insertBand(double *band);
    void toggleVertical();

  private:
    bool vertical = 0;
    size_t currBandPos = 0;
    size_t dataCount = 0;
    size_t scale = 0;

    ColorArray colors;
    std::shared_ptr<std::vector<SDL_Rect>> rects;

    SDL_Rect **vRects;
    double **bands;
};

#endif
