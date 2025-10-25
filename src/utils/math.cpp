#include "utils/math.hpp"

#include <cmath>

std::vector<std::pair<int, int>> bresenham(const int x0, const int y0,
                                           const int x1, const int y1) {
  std::vector<std::pair<int, int>> points;

  if (x1 - x0 > 0 && std::abs(y1 - y0) <= std::abs(x1 - x0)) {
    // Gentle slope, left to right
    for (int x = x0; x <= x1; ++x) {
      int y = std::round(y0 + (y1 - y0) * (x - x0) / (x1 - x0));
      points.emplace_back(x, y);
    }
  } else if (x0 - x1 > 0 && std::abs(y1 - y0) <= std::abs(x1 - x0)) {
    // Gentle slope, right to left
    for (int x = x0; x >= x1; --x) {
      int y = std::round(y0 + (y1 - y0) * (x - x0) / (x1 - x0));
      points.emplace_back(x, y);
    }
  } else if (y1 - y0 > 0 && std::abs(x1 - x0) < std::abs(y1 - y0)) {
    // Steep slope, bottom to top
    for (int y = y0; y <= y1; ++y) {
      int x = std::round(x0 + (x1 - x0) * (y - y0) / (y1 - y0));
      points.emplace_back(x, y);
    }
  } else if (y0 - y1 > 0 && std::abs(x1 - x0) < std::abs(y1 - y0)) {
    // Steep slope, top to bottom
    for (int y = y0; y >= y1; --y) {
      int x = std::round(x0 + (x1 - x0) * (y - y0) / (y1 - y0));
      points.emplace_back(x, y);
    }
  } else {
    // Single point
    points.emplace_back(x0, y0);
  }

  return points;
}

int sign(int val) { return (0 < val) - (val < 0); }
