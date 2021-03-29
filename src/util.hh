#include <initializer_list>
#include <tuple>

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

template<typename T>
class List {
public:
  List(std::initializer_list<T>&& args) {
    data.insert(data.begin(), args.begin(), args.end());
  }

  T& operator[](size_t index) {
    return data[index];
  }

private:
  std::vector<T> data;
};

typedef std::tuple<int, int, int> ColorTuple;
typedef std::vector<double> Band;

inline bool isBandEmpty(const Band& band) {
  for (auto d : band) {
    if (d != 0.0) return false;
  }

  return true;
}

#endif
