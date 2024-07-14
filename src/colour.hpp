#ifndef COLOUR_HPP
#define COLOUR_HPP

#include "interval.hpp"
#include "vec3.hpp"

// note that we are using vec3 for both coloUrs and geometry
using Colour = Vec3;

inline double linearToGamma(double linearComponent) {
  if (linearComponent > 0) {
    return sqrt(linearComponent);
  }
  return 0;
}

void writeColour(std::ostream &out, const Colour &pixelColour) {
  auto r = pixelColour.x();
  auto g = pixelColour.y();
  auto b = pixelColour.z();

  // linear->gamma transform for gamma 2
  r = linearToGamma(r);
  g = linearToGamma(g);
  b = linearToGamma(b);

  // translate values from [0,1] -> [0,255]
  static const Interval intensity(0.000, 0.999);
  int rbyte = int(255.999 * intensity.clamp(r));
  int gbyte = int(255.999 * intensity.clamp(g));
  int bbyte = int(255.999 * intensity.clamp(b));

  // write out the components as defined in PPM format
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif