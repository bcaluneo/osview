// Copyright (C) Brendan Caluneo

// Header Name:
// - Utility
// Description:
// - Holds some constants and other utility functions.

#include <initializer_list>
#include <tuple>
#include <cmath>
#include <plog/Log.h>
#include <array>

#ifndef UTIL_HH
#define UTIL_HH

const std::string CPU_USER_STRING = "user";
const std::string CPU_KERNEL_STRING = "kernel";
const std::string CPU_IDLE_STRING = "idle";

const std::string MEM_FREE_STRING = "free";
const std::string MEM_INUSE_STRING = "inuse";

const size_t BAR_X = 10;
const size_t BAR_Y = 40;
const size_t BAR_SCALE = 4;
const size_t BAR_WIDTH = 380;
const size_t BAR_HEIGHT = 30;
const size_t TARGET_TIME = 60;
const size_t BAND_WIDTH = 4;
const size_t TOTAL_BANDS = BAR_WIDTH / BAND_WIDTH;
const size_t COLUMN_COUNT = 2;
const size_t POLL_TIME = (TARGET_TIME*1000)/TOTAL_BANDS;

const size_t VERTICAL_SPACING = 35;
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
typedef std::vector<std::pair<std::string, signed>> Band;

// Description:
// - Determines whether a given data band is empty.
// - A band is considered "empty" is all data points are 0.0.
// Arguments:
// - band - the band to check
// Result:
// - true if empty, false if not.
inline bool isBandEmpty(const Band& band) {
  for (auto pair : band) {
    if (pair.second != 0.0) return false;
  }

  return true;
}

#endif
