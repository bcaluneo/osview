#include "graph.hh"
#include <iostream>

Graph::Graph(int dataCount, int scale) : _dataCount(dataCount) {
  rects = new std::pair<SDL_Rect, int*>[dataCount];

  for (int i = 0; i < _dataCount; i++) {
    std::get<0>(rects[i]).x = BAR_X;
    std::get<0>(rects[i]).y = scale == 0 ? BAR_Y : BAR_Y*scale*BAR_SCALE;
    std::get<0>(rects[i]).h = BAR_HEIGHT;
  }
}

Graph::~Graph() {
  delete [] rects;
}

void Graph::draw() {
  if (vertical) {
  } else {
    for (int i = 0; i < _dataCount; i++) {
      SDL_Rect &rect = std::get<0>(rects[i]);
      int *colors = std::get<1>(rects[i]);

      SDL_SetRenderDrawColor(render, colors[0], colors[1], colors[2], 255);
      SDL_RenderFillRect(render, &rect);
    }
  }
}

// TODO: Rework this to make it general instead of working for 3.
void Graph::updateSize(int index, double amount) {
  SDL_Rect &rect = std::get<0>(rects[index]);
  rect.w = BAR_WIDTH * (amount/100);
  if (index == 1) rect.x = BAR_X + std::get<0>(rects[0]).w;
  if (index == 2) rect.x = std::get<0>(rects[1]).x + std::get<0>(rects[1]).w;

  if (bands.size() >= TOTAL_BANDS) {
  	bands.insert(bands.begin(), amount);
  	bands.erase(bands.end());
  } else {
  	bands.push_back(amount);
  }
}

void Graph::setColors(int colors[][3]) {
  for (int i = 0; i < _dataCount; i++) {
    std::get<1>(rects[i]) = colors[i];
  }
}
