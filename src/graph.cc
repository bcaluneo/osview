// Copyright (C) Brendan Caluneo

#include "graph.hh"

std::mutex dataLock, bandLock;

inline void drawTextWithShadow(SDL_Renderer *render, std::string msg, NFont &font, int x, int y, NFont::Color col) {
	font.draw(render, x + 2, y + 2, NFont::Color(0, 0, 0, 255), msg.c_str());
	font.draw(render, x, y, col, msg.c_str());
}

Graph::Graph(std::string title, std::map<std::string, ColorTuple> dataInfo)
                       : title(title), dataInfo(dataInfo) {}

// Description:
// - Draws the bar graph.
// Arguments:
// - render - a pointer to an SDL_Renderer instance
// Result:
// - none
void Graph::draw(SDL_Point pos, SDL_Texture *barTexture, NFont &font, SDL_Renderer *render) {
  SDL_Rect barRect {pos.x - 1, pos.y - 1, BAR_WIDTH + 2, BAR_HEIGHT + 2};
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderDrawRect(render, &barRect);

  if (isVertical) {
    std::lock_guard<std::mutex> lock(bandLock);
    for (size_t i = 0; i < bands.size(); ++i) {
      if (isBandEmpty(bands[i])) continue;

      auto band = bands[i];
      auto previousHeight = 0;
      for (auto pair : band) {
        auto key = pair.first;
        auto height = pair.second;
        if (height == 0) continue;
        SDL_Rect rect {static_cast<signed>(pos.x + (i * BAND_WIDTH)), previousHeight + pos.y, BAND_WIDTH, height};
        previousHeight += height;
        const auto [r, g, b] = dataInfo[key];
        SDL_SetRenderDrawColor(render, r, g, b, 255);
        SDL_RenderFillRect(render, &rect);
      }
    }
  } else {
    auto previousWidth = 0;
    std::lock_guard<std::mutex> lock(dataLock);
    for (const auto& [key, width] : data) {
      if (width == 0) continue;
      SDL_Rect rect {previousWidth + pos.x, pos.y, width, BAR_HEIGHT};
      previousWidth += width;


      const auto& [r, g, b] = dataInfo[key];
      SDL_SetRenderDrawColor(render, r, g, b, 255);
      SDL_RenderFillRect(render, &rect);
    }
  }

  //Draw the lower rect
  auto lowerRectHeight = 8;
  SDL_Rect lowerRect {pos.x - 1, pos.y + BAR_HEIGHT + 1, BAR_WIDTH + 2, lowerRectHeight};
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderFillRect(render, &lowerRect);

  auto tickCount = 10;
  auto tickSpace = BAR_WIDTH / tickCount;
  for (size_t i = 1; i < tickCount; ++i) {
    SDL_Rect lowerRect {pos.x - 1 + (i*tickSpace), pos.y + BAR_HEIGHT + 1, 1, lowerRectHeight / 2};
    SDL_SetRenderDrawColor(render, 255, 216, 0, 255);
    SDL_RenderFillRect(render, &lowerRect);
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
void Graph::setData(std::string key, signed amount) {
  std::lock_guard<std::mutex> lock(dataLock);
  data[key] = amount;
}

// Description:
// - Inserts a band into the vertical graph and scrolls
// - all of the other bands.
// Arguments:
// - band - the band to be inserted
// Result:
// - none
void Graph::addBand(Band&& band) {
  std::lock_guard<std::mutex> lock(bandLock);
  if (bands.size() >= TOTAL_BANDS) {
    for (size_t i = bands.size() - 1; i > 0; --i) {
      bands[i] = bands[i - 1];
    }
    
    bands[0] = band;
  } else {
    bands.insert(bands.begin(), band);
  }
}

// Description:
// - Toggles the vertical state.
// Arguments:
// - none
// Result:
// - none
void Graph::toggleVertical() {
  isVertical = !isVertical;
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
  std::lock_guard<std::mutex> lock(bandLock);
  for (auto b : bands) {
    if (isBandEmpty(b)) break;
    result++;
  }

  return result;
}

std::string Graph::getTitle() {
  return this->title;
}