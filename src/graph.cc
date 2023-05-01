// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

inline void drawTextWithShadow(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	font.draw(render, x + 2, y + 2, NFont::Color(0, 0, 0, 255), msg.c_str());
	font.draw(render, x, y, col, msg.c_str());
}

Graph::Graph(std::string title, std::map<std::string, ColorTuple> dataInfo)
                       : title(title), dataInfo(dataInfo) {
  for (size_t i = 0; i < TOTAL_BANDS; ++i) {
    Band band;
    for (size_t j = 0; j < dataInfo.size(); ++j) { band.push_back(0.0); }
    bands.push_back(band);
  }
}

// Description:
// - Draws the bar graph.
// Arguments:
// - render - a pointer to an SDL_Renderer instance
// Result:
// - none
void Graph::draw(SDL_Point pos, SDL_Texture *barTexture, NFont &font, SDL_Renderer *render) {
  SDL_Rect barRect {pos.x - 1, pos.y - 1, BAR_WIDTH + 2, BAR_HEIGHT + 3};
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderDrawRect(render, &barRect);

  if (vertical) {
    // SDL_Rect bg {pos.x, pos.y, static_cast<signed>(getFilledBandSize()*BAND_WIDTH), BAR_HEIGHT + 1};
    // const auto [r, g, b] = colors[szData - 1];
    // SDL_SetRenderDrawColor(render, r, g, b, 255);
    // SDL_RenderFillRect(render, &bg);

    // for (size_t i = 0; i < bands.size(); ++i) {
    //   if (isBandEmpty(bands[i])) continue;
    //   for (size_t j = 0; j < szData; j++) {
    //     SDL_Rect rect {static_cast<signed>(pos.x + (i * BAND_WIDTH)), pos.y, BAND_WIDTH, static_cast<signed>(BAR_HEIGHT * (bands[i][j]/100))};

    //     if (j > 0) {
    //       for (int k = j-1; k >= 0; --k) {
    //         rect.y += (BAR_HEIGHT * (bands[i][k]/100));
    //       }
    //     }

    //     const auto [r, g, b] = colors[j];
    //     SDL_SetRenderDrawColor(render, r, g, b, 255);
    //     SDL_RenderFillRect(render, &rect);
    //   }
    // }
  } else {
    auto previousWidth = 0;
    for (const auto& [key, data] : data) {
      auto width = static_cast<signed>(BAR_WIDTH * (data/100));
      SDL_Rect rect {previousWidth + pos.x, pos.y, width, BAR_HEIGHT + 1};
      previousWidth += width;

      const auto [r, g, b] = dataInfo[key];
      SDL_SetRenderDrawColor(render, r, g, b, 255);
      SDL_RenderFillRect(render, &rect);
    }
  }

  drawTextWithShadow(render, title.c_str(), font, pos.x, pos.y - font.getHeight() - 6, NFont::Color(255, 255, 255, 255));
  
  auto titleWidth = font.getWidth(title.append("\t").c_str());
  for (const auto& [key, color] : dataInfo) {
    const auto [r, g, b] = color;
    drawTextWithShadow(render, key, font, pos.x + titleWidth, pos.y - font.getHeight() - 6, NFont::Color(r, g, b, 255));
    
    // Not sure why append doesn't work here.
    std::string ap = key + "\t";
    titleWidth += font.getWidth(ap.c_str());
  }
}

// Description:
// - Sets a data point at its index.
// Arguments:
// - index - the index of the data point to set
// - amount - the value to set
// Result:
// - none
void Graph::setData(std::string key, double amount) {
  data[key] = amount;
}

// Description:
// - Inserts a band into the vertical graph and scrolls
// - all of the other bands.
// Arguments:
// - band - the band to be inserted
// Result:
// - none
void Graph::pushBand(Band&& band) {
  for (size_t i = bands.size() - 1; i > 0; --i) {
    bands[i] = bands[i - 1];
  }

  bands[0] = band;
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

std::string Graph::getTitle() {
  return this->title;
}