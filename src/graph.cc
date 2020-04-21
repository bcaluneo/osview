#include "graph.hh"
#include <iostream>

Graph::Graph(size_t dataCount, size_t scale) : _dataCount(dataCount) {
  rects = new SDL_Rect[dataCount];
  rects2 = new SDL_Rect*[TOTAL_BANDS];

  bands = new std::vector<double*>();

  bands->reserve(TOTAL_BANDS);
  colors.reserve(dataCount);

  for (size_t i = 0; i < dataCount; i++) {
    rects[i].x = BAR_X;
    rects[i].y = scale == 0 ? BAR_Y : BAR_Y*scale*BAR_SCALE;
    rects[i].h = BAR_HEIGHT;
  }

  for (size_t i = 0; i < TOTAL_BANDS; i++) {
    for (size_t j = 0; j < dataCount; j++) {
      rects2[i] = new SDL_Rect[3];
      rects2[i][j].w = BAND_WIDTH;
    }
  }
}

Graph::~Graph() {
  for (size_t i = 0; i < _dataCount; i++) {
    delete [] rects2[i];
  }

  delete bands;
  delete [] rects;
  delete [] rects2;
}

void Graph::draw() {
  if (vertical) {
    for (auto it = bands->begin(); it != bands->end(); it++) {
      auto pos = it-bands->begin();

      double *data = (*bands)[pos];

      double usrValue = data[0];
      double kerValue = data[1];
      double idlValue = data[2];

      SDL_Rect &usrRect = rects2[pos][0];
      SDL_Rect &kerRect = rects2[pos][1];
      SDL_Rect &idlRect = rects2[pos][2];

      usrRect.x = BAR_X + pos*BAND_WIDTH;
      kerRect.x = BAR_X + pos*BAND_WIDTH;
      idlRect.x = BAR_X + pos*BAND_WIDTH;
      usrRect.w = kerRect.w = idlRect.w = BAND_WIDTH;
      usrRect.h = BAR_HEIGHT * (usrValue/100);
      kerRect.h = BAR_HEIGHT * (kerValue/100);
      idlRect.h = BAR_HEIGHT * (idlValue/100);
      usrRect.y = BAR_Y + BAR_HEIGHT - usrRect.h;
      kerRect.y = usrRect.y - kerRect.h;
      idlRect.y = kerRect.y - idlRect.h;

      SDL_SetRenderDrawColor(render, 0, 148, 255, 255);
      SDL_RenderFillRect(render, &usrRect);
      SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
      SDL_RenderFillRect(render, &kerRect);
      SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
      SDL_RenderFillRect(render, &idlRect);
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

// TODO: Rework this to make it general instead of working for 3.
void Graph::updateSize(int index, double amount) {
  if (vertical) return;
  SDL_Rect &rect = rects[index];
  rect.w = BAR_WIDTH * (amount/100);

  if (index == 1) rect.x = BAR_X + rects[0].w;
  if (index == 2) rect.x = rects[1].x + rects[1].w;

}

void Graph::insertBand(double *band) {
  printf("%f\t%f\t%f\n", band[0], band[1], band[2]);

  if (bands->size() >= TOTAL_BANDS) {
  	// bands->insert(bands->begin(), band);
  	// bands->erase(bands->end()-1);
  } else {
    bands->insert(bands->end(), band);
  }
}

void Graph::setColors(int colors[][3]) {
  for (size_t i = 0; i < _dataCount; i++) {
    this->colors.insert(this->colors.end(), colors[i]);
  }
}

void Graph::toggleVertical() {
  vertical = 1 - vertical;
}
