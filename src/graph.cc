// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

#define Y(scale) static_cast<signed>((scale == 0 ? BAR_Y : 1+BAR_Y*scale*BAR_SCALE))

Graph::Graph(std::string title, size_t dataCount, size_t scale, List<ColorTuple> colors)
                        : title(title), dataCount(dataCount), scale(scale), colors(colors) {
  data.resize(dataCount);
  for (size_t i = 0; i < TOTAL_BANDS; ++i) {
    Band band;
    for (size_t j = 0; j < dataCount; ++j) { band.push_back(0.0); }
    bands.push_back(band);
  }
}

void Graph::draw(SDL_Renderer *render) {
  if (vertical) {
    SDL_Rect bg {BAR_X, Y(scale), static_cast<signed>(getFilledBandSize()*BAND_WIDTH), BAR_HEIGHT};
    const auto [r, g, b] = colors[dataCount - 1];
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &bg);

    for (size_t i = 0; i < bands.size(); ++i) {
      if (isBandEmpty(bands[i])) continue;
      for (size_t j = 0; j < dataCount; j++) {
        SDL_Rect rect {static_cast<signed>(BAR_X + (i * BAND_WIDTH)), Y(scale), BAND_WIDTH, static_cast<signed>(BAR_HEIGHT * (bands[i][j]/100))};

        if (j > 0) {
          for (int k = j-1; k >= 0; --k) {
            rect.y += (BAR_HEIGHT * (bands[i][k]/100));
          }
        }

        const auto [r, g, b] = colors[j];
        SDL_SetRenderDrawColor(render, r, g, b, 255);
        SDL_RenderFillRect(render, &rect);
      }
    }
  } else {
    SDL_Rect bg {BAR_X, Y(scale), BAR_WIDTH, BAR_HEIGHT};
    const auto [r, g, b] = colors[dataCount - 1];
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &bg);

    for (size_t i = 0; i < dataCount; i++) {
      SDL_Rect rect {BAR_X, Y(scale), static_cast<signed>(BAR_WIDTH * (data[i]/100)), BAR_HEIGHT + 1};

      if (i > 0) {
        for (int j = i-1; j >= 0; --j) {
          rect.x += (BAR_WIDTH * (data[j]/100));
        }
      }

      const auto [r, g, b] = colors[i];
      SDL_SetRenderDrawColor(render, r, g, b, 255);
      SDL_RenderFillRect(render, &rect);
    }
  }
}

void Graph::setData(int index, double amount) {
  data[index] = amount;
}

void Graph::insertBand(Band&& band) {
  if (bands.size() >= TOTAL_BANDS) {
    bands[currBandPos++] = band;
    for (size_t i = bands.size() - 1; i > currBandPos; --i) {
      bands[i] = bands[i - 1];
    }

    if (currBandPos >= TOTAL_BANDS) currBandPos = 0;
  }

  band.clear();
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}

size_t Graph::getFilledBandSize() {
  size_t result = 0;
  for (auto b : bands) {
    if (isBandEmpty(b)) break;
    result++;
  }

  return result;
}
