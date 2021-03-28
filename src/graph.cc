// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

Graph::Graph(std::string title, size_t dataCount, size_t scale, ColorArray colors)
                        : title(title), dataCount(dataCount), scale(scale), colors(colors) {
  rects = std::make_shared<std::vector<SDL_Rect>>();
  bands = std::make_shared<std::vector<Band>>();
  rects->resize(dataCount);
}

void Graph::draw(SDL_Renderer *render) {
  if (vertical) {
    for (auto it = bands->begin(); it != bands->end(); it++) {
      for (size_t i; i < dataCount; i++) {
        auto pos = it - bands->begin();
        auto band = *it;
        auto &rect = band.at(i);
        rect.y = (scale == 0 ? BAR_Y : BAR_Y*scale*BAR_SCALE) + BAR_HEIGHT - rect.h;
        rect.x += pos*BAND_WIDTH;

        if (i > 0) {
          auto &pRect = band.at(i-i);
          rect.y = pRect.y - rect.h;
        }

        const auto [cr, g, b] = colors.at(i);
        SDL_SetRenderDrawColor(render, cr, g, b, 255);
        SDL_RenderFillRect(render, &rect);
      }
    }
  } else {
    for (size_t i = 0; i < dataCount; i++) {
      const auto [r, g, b] = colors.at(i);
      SDL_SetRenderDrawColor(render, r, g, b, 255);
      SDL_RenderFillRect(render, &rects->at(i));
    }
  }
}

void Graph::setData(int index, double amount) {
  SDL_Rect &rect = rects->at(index);
  rect.x = BAR_X;
  rect.w = BAR_WIDTH * (amount/100);
  rect.y = scale == 0 ? BAR_Y : 1 + BAR_Y*scale*BAR_SCALE;
  rect.h = BAR_HEIGHT + 1;

  if (index == 1) {
    rect.x = BAR_X + rects->at(0).w;
  } else if (index > 1) {
    rect.x = rects->at(index-1).x + rects->at(index-1).w;
  }
}

void Graph::insertBand(const Band& band) {
  bands->push_back(band);
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}
