#include <iostream>

#include "colour.hpp"
#include "vec3.hpp"

int main() {
  // image
  static constexpr int IMAGE_WIDTH = 256;
  static constexpr int IMAGE_HEIGHT = 256;

  // render
  std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

  for (int m = 0; m < IMAGE_HEIGHT; ++m) {
    std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - m) << ' '
              << std::flush;
    for (int n = 0; n < IMAGE_WIDTH; ++n) {
      auto pixelColour = colour(double(n) / (IMAGE_WIDTH - 1),
                                double(m) / (IMAGE_HEIGHT - 1), 0);
      writeColour(std::cout, pixelColour);
    }
  }
  std::clog << "\rDone.                 \n";
}
