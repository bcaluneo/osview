// Copyright (C) Brendan Caluneo

// Header Name:
// - Graph
// Description:
// - Renders a horizontal, and toggleable vertical, bar graph of an
// - arbitrary amount of data points with specificed colors.

#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include "SDL.h"
#include "SDL_image.h"
#include "util.hh"
#include <plog/Log.h>
#include "NFont.h"
#include <map>
#include <mutex>

#ifndef GRAPH_HH
#define GRAPH_HH

class Graph {
  public:
    Graph(std::string title, std::map<std::string, ColorTuple> dataInfo);

    void draw(SDL_Point pos, SDL_Texture *barTexture, NFont &font, SDL_Renderer *render);
    void setData(std::string key, signed amount);
    void addBand(Band&& band);
    void toggleVertical();
    std::string getTitle();

  private:

    std::string title;
    unsigned szData = 0;
    bool isVertical = false;

    std::map<std::string, ColorTuple> dataInfo;
    std::map<std::string, signed> data;
    std::vector<Band> bands;

    size_t getFilledBandSize();
};

#endif
