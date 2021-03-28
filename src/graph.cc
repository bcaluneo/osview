// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

Graph::Graph(std::string title, size_t dataCount, size_t scale, ColorArray colors)
                        : title(title), dataCount(dataCount), scale(scale), colors(colors) {
  data.resize(dataCount);
}

// TODO: There's a bug here that's causing a segfault.
void Graph::draw(SDL_Renderer *render) {
  if (vertical) {
    for (size_t i = 0; i < bands.size(); ++i) {
      for (size_t j = 0; j < dataCount; j++) {
        SDL_Rect rect;
        rect.x = BAR_X + (i * BAND_WIDTH);
        rect.w = BAND_WIDTH;
        rect.h = BAR_HEIGHT * (bands[i].at(j)/100);
        rect.y = (scale == 0 ? BAR_Y : BAR_Y*scale*BAR_SCALE);

        if (j > 0) {
          for (int k = j-1; k >= 0; --k) {
            rect.y += (BAR_HEIGHT * (bands[i].at(k)/100));
          }
        }

        const auto [cr, g, b] = colors.at(j);
        SDL_SetRenderDrawColor(render, cr, g, b, 255);
        SDL_RenderFillRect(render, &rect);
      }
    }
  } else {
    for (size_t i = 0; i < dataCount; i++) {
      const auto [r, g, b] = colors.at(i);
      SDL_SetRenderDrawColor(render, r, g, b, 255);
      SDL_Rect rect;
      rect.x = BAR_X;
      rect.w = BAR_WIDTH * (data[i]/100);
      rect.y = scale == 0 ? BAR_Y : 1 + BAR_Y*scale*BAR_SCALE;
      rect.h = BAR_HEIGHT + 1;

      if (i > 0) {
        for (int j = i-1; j >= 0; --j) {
          rect.x += (BAR_WIDTH * (data[j]/100));
        }
      }

      SDL_RenderFillRect(render, &rect);
    }
  }
}

void Graph::setData(int index, double amount) {
  data[index] = amount;
}

void Graph::insertBand(const Band& band) {
  if (bands.size() >= TOTAL_BANDS) {
    bands[currBandPos++] = band;
    bands[TOTAL_BANDS - 1] = bands[0];
    if (currBandPos >= TOTAL_BANDS) currBandPos = 0;
  } else {
    bands.push_back(band);
  }
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}
