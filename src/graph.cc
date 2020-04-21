#include "graph.hh"
#include <iostream>

Graph::Graph(size_t dataCount, size_t scale) : _dataCount(dataCount) {
  rects = new SDL_Rect[dataCount];
  rects2 = new SDL_Rect*[TOTAL_BANDS];
  bands = new double*[TOTAL_BANDS];

  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    bands[i] = new double[dataCount];

    for (size_t j = 0; j < dataCount; j++) {
      bands[i][j] = 0.0;
    }
  }

  colors.reserve(dataCount);

  for (size_t i = 0; i < dataCount; i++) {
    rects[i].x = BAR_X;
    rects[i].y = scale == 0 ? BAR_Y : BAR_Y*scale*BAR_SCALE;
    rects[i].h = BAR_HEIGHT;
  }

  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    for (size_t j = 0; j < dataCount; j++) {
      rects2[i] = new SDL_Rect[3];
    }
  }

  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    for (size_t j = 0; j < dataCount; j++) {
      rects2[i][j].w = BAND_WIDTH;
    }
  }
}

Graph::~Graph() {
  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    delete [] bands[i];

    for (size_t i = 0; i < _dataCount; i++) {
      delete [] rects2[i];
    }
  }


  delete [] bands;
  delete [] rects;
  delete [] rects2;
}

void Graph::draw() {
  if (vertical) {
    for (size_t i = 0; i < TOTAL_BANDS; i++) {
      double *data = bands[i];
      if (data[0] == 0 && data[1] == 0 && data[2] == 0) continue;

      for (size_t j = 0; j < _dataCount; j++) {
        SDL_Rect &rect = rects2[i][j];
        int *cols = colors[i];
        rect.x = BAR_X + i*BAND_WIDTH;

        rect.y = BAR_Y + BAR_HEIGHT - rect.h;
        rect.h = BAR_HEIGHT * (data[j] / 100);

        // else {
        //   rect.y = rects2[i][j-1].y - rect.h;
        // }
        //
        SDL_SetRenderDrawColor(render, cols[0], cols[1], cols[2], 255);
        SDL_RenderFillRect(render, &rect);
      }
    }
  } else {
    for (size_t i = 0; i < _dataCount; i++) {
      SDL_Rect &rect = rects[i];
      int *cols = colors[i];

      SDL_SetRenderDrawColor(render, cols[0], cols[1], cols[2], 255);
      SDL_RenderFillRect(render, &rect);
    }
  }
}

void Graph::updateSize(int index, double amount) {
  if (vertical) return;
  SDL_Rect &rect = rects[index];
  rect.w = BAR_WIDTH * (amount/100);

  if (index == 1) {
    rect.x = BAR_X + rects[0].w;
  } else if (index > 1) {
    if (index == 2) rect.x = rects[index-1].x + rects[index-1].w;
  }
}

void Graph::insertBand(double *band) {
  if (currBandPos >= TOTAL_BANDS) {
    currBandPos = 0;
  }

  for (size_t j = 0; j < _dataCount; j++) {
    bands[currBandPos][j] = band[j];
  }

  currBandPos++;
}

void Graph::setColors(int colors[][3]) {
  for (size_t i = 0; i < _dataCount; i++) {
    this->colors.insert(this->colors.end(), colors[i]);
  }
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}
