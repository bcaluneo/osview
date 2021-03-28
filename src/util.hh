#include <initializer_list>

#ifndef UTIL_HH
#define UTIL_HH

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

typedef std::tuple<int, int, int> ColorTuple;

class ColorArray {

public:
  ColorArray(std::initializer_list<ColorTuple>&& t) {
    colors.insert(colors.end(), t.begin(), t.end());
  }

  auto& at(size_t index) {
    return colors.at(index);
  }

private:
  std::vector<ColorTuple> colors;
};

class Band {

public:
  Band(std::initializer_list<double>&& list) {
    data.insert(data.end(), list.begin(), list.end());
  }

  auto& at(size_t index) {
    return data[index];
  }

private:
  std::vector<double> data;
};

#endif
