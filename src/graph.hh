#include <vector>
#include <tuple>
#include "SDL.h"

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(int dataCount);
    ~Graph();
    bool vertical;
    int _dataCount;
    SDL_Rect *rects;
    std::vector<double> bands;

    void draw();
};

#endif
