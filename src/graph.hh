// Copyright (C) Brendan Caluneo

#include <vector>
#include "SDL.h"
#include "util.hh"

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(std::string title, size_t dataCount, size_t scale, List<ColorTuple> colors);

    void draw(SDL_Renderer *render);
    void setData(int index, double amount);
    void insertBand(Band&& band);
    void toggleVertical();

  private:
    std::string title;
    bool vertical = 0;
    size_t currBandPos = 0;
    size_t dataCount = 0;
    size_t scale = 0;

    List<ColorTuple> colors;
    std::vector<double> data;
    std::vector<Band> bands;

    size_t getFilledBandSize();
};

#endif
