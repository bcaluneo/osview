// Copyright (C) Brendan Caluneo

// Header Name:
// - Utility
// Description:
// - Holds some constants and other utility functions.

#include <initializer_list>
#include <tuple>

#ifndef UTIL_HH
#define UTIL_HH

constexpr size_t BAR_X = 10;
constexpr size_t BAR_Y = 40;
constexpr size_t BAR_SCALE = 4;
constexpr size_t BAR_WIDTH = 380;
constexpr size_t BAR_HEIGHT = 50;
constexpr size_t POLL_TIME = 250;
constexpr size_t BAND_WIDTH = 4;
constexpr size_t TOTAL_BANDS = BAR_WIDTH / BAND_WIDTH;
constexpr size_t COLUMN_COUNT = 2;

const size_t VERTICAL_SPACING = 50;
const size_t HORIZONTAL_SPACING = 50;

// Description:
// - List, wrapper around a vector of same-type arguments
// Arguments:
// - An arbitrary number of elements of the same-type.
// Result:
// - Produces a read-only vector of same-type elements.
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

// typedefs for commonly used types.
typedef std::tuple<int, int, int> ColorTuple;
typedef std::vector<double> Band;

// Description:
// - Determines whether a given data band is empty.
// - A band is considered "empty" is all data points are 0.0.
// Arguments:
// - band - the band to check
// Result:
// - true if empty, false if not.
inline bool isBandEmpty(const Band& band) {
  for (auto d : band) {
    if (d != 0.0) return false;
  }

  return true;
}

#endif
