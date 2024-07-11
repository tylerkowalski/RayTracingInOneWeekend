#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend.hpp"

#include "hittable.hpp"

class Camera {
public:
  // rather than using a constructor or setters, these 2 fields are public and
  // define how intialize() will set the other variables
  double ASPECT_RATIO = 1.0; // default image width / image height
  int IMAGE_WIDTH = 100;     // default value for image width

  void render(const Hittable &world) {
    initialize();

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int m = 0; m < IMAGE_HEIGHT; ++m) {
      std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - m) << ' '
                << std::flush;
      for (int n = 0; n < IMAGE_WIDTH; ++n) {
        auto pixelCentre =
            PIXEL00_LOC + (n * PIXEL_DELTA_U) + (m * PIXEL_DELTA_V);
        auto rayDirection = pixelCentre - CAMERA_CENTRE;
        Ray r(CAMERA_CENTRE, rayDirection);

        Colour pixelColour = rayColour(r, world);
        writeColour(std::cout, pixelColour);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  // these private fields are defined based on initialize()
  int IMAGE_HEIGHT;     // rendered image height
  Point3 CAMERA_CENTRE; // camera centre
  Point3 PIXEL00_LOC;   // location of pixel 0,0
  Vec3 PIXEL_DELTA_U;   // vector to pixel to the right
  Vec3 PIXEL_DELTA_V;   // vector to pixel below

  void initialize() {
    // calculate the image height with min val = 1
    IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
    IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

    CAMERA_CENTRE = Point3(0, 0, 0);

    // determine viewport dimensions
    auto FOCAL_LENGTH = 1.0;
    auto VIEWPORT_HEIGHT = 2.0; // arbitrary
    auto VIEWPORT_WIDTH =
        VIEWPORT_HEIGHT * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);

    // vectors that define the viewport
    auto VIEWPORT_U = Vec3(VIEWPORT_WIDTH, 0, 0);
    auto VIEWPORT_V =
        Vec3(0, -VIEWPORT_HEIGHT, 0); // we want the v axis to be going "down"

    // pixel distance vectors
    PIXEL_DELTA_U = VIEWPORT_U / IMAGE_WIDTH;
    PIXEL_DELTA_V = VIEWPORT_V / IMAGE_HEIGHT;

    // position of upper-left of the viewport
    auto VIEWPORT_UPPER_LEFT = CAMERA_CENTRE - Vec3(0, 0, FOCAL_LENGTH) -
                               (VIEWPORT_U / 2) - (VIEWPORT_V / 2);
    PIXEL00_LOC =
        VIEWPORT_UPPER_LEFT + (PIXEL_DELTA_U / 2) + (PIXEL_DELTA_V / 2);
  }

  Colour rayColour(const Ray &r, const Hittable &world) const {
    HitRecord rec;
    if (world.hit(r, Interval(0, infinity), rec)) {
      return 0.5 * (rec.normal + Colour(1, 1, 1));
    }

    Vec3 unitDirection = unitVector(r.direction());
    auto alpha = 0.5 * (unitDirection.y() + 1.0); // puts alpha between 0 and 1
    return (1.0 - alpha) * Colour(1.0, 1.0, 1.0) +
           alpha * Colour(0.5, 0.7,
                          1.0); // linear interpolation between blue and white
  }
};

#endif