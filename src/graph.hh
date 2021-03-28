// Copyright (C) Brendan Caluneo

#include <vector>
#include <memory>
#include "SDL.h"
#include "util.hh"

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(std::string title, size_t dataCount, size_t scale, ColorArray colors);

    void draw(SDL_Renderer *render);
    void setData(int index, double amount);
    void insertBand(const Band& band);
    void toggleVertical();

  private:
    std::string title;
    bool vertical = 0;
    size_t currBandPos = 0;
    size_t dataCount = 0;
    size_t scale = 0;

    ColorArray colors;
    std::shared_ptr<std::vector<SDL_Rect>> rects;
    std::shared_ptr<std::vector<Band>> bands;
};

#endif
