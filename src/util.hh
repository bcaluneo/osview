#include <initializer_list>

#ifndef UTIL_HH
#define UTIL_HH

typedef std::tuple<int, int, int> ColorTuple;

class ColorArray {

public:
  ColorArray(std::initializer_list<ColorTuple>&& t) {
    colors.insert(colors.end(), t.begin(), t.end());
  }

  std::vector<ColorTuple> colors;
};

constexpr size_t SCREEN_WIDTH = 640;
constexpr size_t SCREEN_HEIGHT = 240;
constexpr size_t BAR_X = 15;
constexpr size_t BAR_Y = 40;
constexpr size_t BAR_SCALE = 4;
constexpr size_t BAR_WIDTH = SCREEN_WIDTH - (BAR_X*2);
constexpr size_t BAR_HEIGHT = 50;
constexpr size_t POLL_TIME = 200;
constexpr size_t BAND_WIDTH = 5;
constexpr size_t TOTAL_BANDS = BAR_WIDTH / BAND_WIDTH;

#endif
