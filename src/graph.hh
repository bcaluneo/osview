// Copyright (C) Brendan Caluneo

// Header Name:
// - Graph
// Description:
// - Renders a horizontal, and toggleable vertical, bar graph of an
// - arbitrary amount of data points with specificed colors.

#include <vector>
#include "SDL.h"
#include "util.hh"

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(std::string title, size_t szData, size_t scale, List<ColorTuple> colors);

    void draw(SDL_Renderer *render);
    void setData(int index, double amount);
    void insertBand(Band&& band);
    void toggleVertical();

  private:
    std::string title;
    bool vertical = 0;
    size_t szData = 0;
    size_t scale = 0;

    List<ColorTuple> colors;
    std::vector<double> data;
    std::vector<Band> bands;

    size_t getFilledBandSize();
};

#endif
