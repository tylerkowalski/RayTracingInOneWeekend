#ifndef COLOUR_H
#define COLOUR_H

#include "vec3.hpp"

#include <iostream>

// note that we are using vec3 for both coloUrs and geometry
using colour = vec3;

void writeColour(std::ostream &out, const colour &pixelColour) {
  auto r = pixelColour.x();
  auto g = pixelColour.y();
  auto b = pixelColour.z();

  // translate values from [0,1] -> [0,255]
  int rbyte = int(255.999 * r);
  int gbyte = int(255.999 * g);
  int bbyte = int(255.999 * b);

  // write out the components as defined in PPM format
  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif