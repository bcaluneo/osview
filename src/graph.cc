// Copyright (C) Brendan Caluneo

#include "graph.hh"
#include <iostream>

Graph::Graph(size_t dataCount, size_t scale) : dataCount(dataCount), scale(scale) {
  rects = std::make_shared<std::vector<SDL_Rect>>();

  vRects = new SDL_Rect*[TOTAL_BANDS];
  bands = new double*[TOTAL_BANDS];

  rects->resize(dataCount);
  colors.reserve(dataCount);

  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    bands[i] = new double[dataCount];
    vRects[i] = new SDL_Rect[dataCount];

    for (size_t j = 0; j < dataCount; j++) {
      bands[i][j] = 0.0;
    }
  }
}

Graph::~Graph() {
  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    delete [] bands[i];
    delete [] vRects[i];
  }

  delete [] vRects;
  delete [] bands;
}

void Graph::draw() {
  if (vertical) {
    for (size_t i = 0; i < TOTAL_BANDS; i++) {
      double *data = bands[i];

      size_t totalZeroes = 0;
      for (size_t j = 0; j < dataCount; j++) {
        if (data[j] == 0.0) totalZeroes += 1;
      }

      if (totalZeroes != dataCount) {
        for (size_t j = 0; j < dataCount; j++) {
          double data = bands[i][j];
          SDL_Rect &r = vRects[i][j];
          int *cols = colors[j];
          r.x = BAR_X + i*BAND_WIDTH;
          r.w = BAND_WIDTH;
          r.h = BAR_HEIGHT * (data/100);

          if (j == 0) {
            r.y =  (scale == 0 ? BAR_Y : 1 + BAR_Y*scale*BAR_SCALE) + BAR_HEIGHT - r.h;
          } else {
            SDL_Rect &r0 = vRects[i][j - 1];
            r.y = r0.y - r.h;
          }

          SDL_SetRenderDrawColor(render, cols[0], cols[1], cols[2], 255);
          SDL_RenderFillRect(render, &r);
        }
      }
    }
  } else {
    for (size_t i = 0; i < dataCount; i++) {
      int *cols = colors[i];

      SDL_SetRenderDrawColor(render, cols[0], cols[1], cols[2], 255);
      SDL_RenderFillRect(render, &rects->at(i));
    }
  }
}

void Graph::updateSize(int index, double amount) {
  if (vertical) return;

  SDL_Rect rect;
  rect.x = BAR_X;
  rect.w = BAR_WIDTH * (amount/100);
  rect.y = scale == 0 ? BAR_Y : 1 + BAR_Y*scale*BAR_SCALE;
  rect.h = BAR_HEIGHT + 1;

  if (index == 1) {
    rect.x = BAR_X + rects->at(0).w;
  } else if (index > 1) {
    rect.x = rects->at(index-1).x + rects->at(index-1).w;
  }

  rects->erase(rects->begin()+index);
  rects->insert(rects->begin()+index, rect);
}

void Graph::insertBand(double *band) {
  if (currBandPos >= TOTAL_BANDS) {
    for (size_t j = 0; j < dataCount; j++) {
      bands[0][j] = band[j];
    }

    for (size_t i = TOTAL_BANDS - 1; i > 0; i--) {
      for (size_t j = 0; j < dataCount; j++) {
        bands[i][j] = bands[i-1][j];
      }
    }

  } else {
    for (size_t j = 0; j < dataCount; j++) {
      bands[currBandPos][j] = band[j];
    }

    currBandPos++;
  }
}

void Graph::setColors(int colors[][3]) {
  for (size_t i = 0; i < dataCount; i++) {
    this->colors.insert(this->colors.end(), colors[i]);
  }
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}
