#include <vector>
#include <tuple>
#include "SDL.h"

#ifndef GRAPH_HH
#define GRAPH_HH

extern SDL_Renderer *render;
extern const int SCREEN_WIDTH;
extern const int BAR_X;
extern const int BAR_Y;
extern const int BAR_SCALE;
extern const int BAR_WIDTH;
extern const int BAR_HEIGHT;
extern const int BAND_WIDTH;

const size_t TOTAL_BANDS = BAR_WIDTH / BAND_WIDTH;

class Graph {
  public:
    Graph(size_t dataCount, size_t scale);
    ~Graph();

    void draw();
    void updateSize(int index, double amount);
    void insertBand(double *band);
    void setColors(int colors[][3]);
    void toggleVertical();

  private:
    bool vertical = 0;
    size_t currBandPos = 0;
    size_t _dataCount = 0;
    SDL_Rect *rects, **vRects;
    double **bands;
    std::vector<int*> colors;
};

#endif
