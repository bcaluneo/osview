// Copyright (C) Brendan Caluneo

// Header Name:
// - Graph
// Description:
// - Renders a horizontal, and toggleable vertical, bar graph of an
// - arbitrary amount of data points with specificed colors.

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "util.hh"

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(std::string title, unsigned szData, SDL_Point pos, List<ColorTuple> colors);

    void draw(SDL_Texture *barTexture, SDL_Renderer *render);
    void setData(int index, double amount);
    void pushBand(Band&& band);
    void toggleVertical();

  private:

    std::string title;
    unsigned szData = 0;
    SDL_Point pos;
    bool vertical = 0;

    List<ColorTuple> colors;
    std::vector<double> data;
    std::vector<Band> bands;

    size_t getFilledBandSize();
};

#endif
