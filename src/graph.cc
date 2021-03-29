// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

// Macro to determine the Y of the graph given its ordering (called scale here)
// TODO: change this.
#define Y(scale) static_cast<signed>((scale == 0 ? BAR_Y : 1+BAR_Y*scale*BAR_SCALE))

Graph::Graph(std::string title, size_t szData, size_t scale, List<ColorTuple> colors)
                        : title(title), szData(szData), scale(scale), colors(colors) {
  data.resize(szData);
  for (size_t i = 0; i < TOTAL_BANDS; ++i) {
    Band band;
    for (size_t j = 0; j < szData; ++j) { band.push_back(0.0); }
    bands.push_back(band);
  }
}

// Description:
// - Draws the bar graph.
// Arguments:
// - render - a pointer to an SDL_Renderer instance
// Result:
// - none
void Graph::draw(SDL_Renderer *render) {
  if (vertical) {
    SDL_Rect bg {BAR_X, Y(scale), static_cast<signed>(getFilledBandSize()*BAND_WIDTH), BAR_HEIGHT + 1};
    const auto [r, g, b] = colors[szData - 1];
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &bg);

    for (size_t i = 0; i < bands.size(); ++i) {
      if (isBandEmpty(bands[i])) continue;
      for (size_t j = 0; j < szData; j++) {
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
    const auto [r, g, b] = colors[szData - 1];
    SDL_SetRenderDrawColor(render, r, g, b, 255);
    SDL_RenderFillRect(render, &bg);

    for (size_t i = 0; i < szData; i++) {
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

// Description:
// - Sets a data point at its index.
// Arguments:
// - index - the index of the data point to set
// - amount - the value to set
// Result:
// - none
void Graph::setData(int index, double amount) {
  data[index] = amount;
}

// Description:
// - Inserts a band into the vertical graph and scrolls
// - all of the other bands.
// Arguments:
// - band - the band to be inserted
// Result:
// - none
void Graph::insertBand(Band&& band) {
  bands[0] = band;
  for (size_t i = bands.size() - 1; i > 0; --i) {
    bands[i] = bands[i - 1];
  }
  band.clear();
}

// Description:
// - Toggles the vertical state.
// Arguments:
// - none
// Result:
// - none
void Graph::toggleVertical() {
  vertical = 1 - vertical;
}

// Description:
// - Gets the index of the band right before an empty one.
// - Used to determine where to start and stop filling the
// - background of the bar with the background color.
// Arguments:
// - none
// Result:
// - the index of the first empty band
size_t Graph::getFilledBandSize() {
  size_t result = 0;
  for (auto b : bands) {
    if (isBandEmpty(b)) break;
    result++;
  }

  return result;
}
