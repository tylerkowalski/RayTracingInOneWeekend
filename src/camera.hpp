#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend.hpp"

#include "hittable.hpp"

class Camera {
public:
  // rather than using a constructor or setters, these 2 fields are public and
  // define how intialize() will set the other variables
  double ASPECT_RATIO = 1.0;  // default image width / image height
  int IMAGE_WIDTH = 100;      // default value for image width
  int SAMPLES_PER_PIXEL = 10; // number of RANDOM samples per pixel
  int MAX_DEPTH = 10;         // maximum number of ray bounces we will allow

  void render(const Hittable &world) {
    initialize();

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int m = 0; m < IMAGE_HEIGHT; ++m) {
      std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - m) << ' '
                << std::flush;
      for (int n = 0; n < IMAGE_WIDTH; ++n) {
        Colour pixelColour(0, 0, 0);
        for (int sample = 0; sample < SAMPLES_PER_PIXEL; ++sample) {
          Ray r = getRay(m, n);
          pixelColour += rayColour(r, MAX_DEPTH, world);
        }
        writeColour(std::cout, PIXEL_SAMPLES_SCALE * pixelColour);
      }
    }
    std::clog << "\rDone.                 \n";
  }

private:
  // these private fields are defined based on initialize()
  int IMAGE_HEIGHT;           // rendered image height
  double PIXEL_SAMPLES_SCALE; // colour scale factor for sum of pixel samples
  Point3 CAMERA_CENTRE;       // camera centre
  Point3 PIXEL00_LOC;         // location of pixel 0,0
  Vec3 PIXEL_DELTA_U;         // vector to pixel to the right
  Vec3 PIXEL_DELTA_V;         // vector to pixel below

  void initialize() {
    // calculate the image height with min val = 1
    IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
    IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

    PIXEL_SAMPLES_SCALE = 1.0 / SAMPLES_PER_PIXEL;

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

  Ray getRay(int m, int n) const {
    // make a ray from origin to a random sample in the region of pixel (i,j)
    auto offset = sampleSquare();
    auto pixelSample = PIXEL00_LOC + (n + offset.x()) * PIXEL_DELTA_U +
                       (m + offset.y()) * PIXEL_DELTA_V;

    auto rayOrigin = CAMERA_CENTRE;
    auto rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
  }

  Vec3 sampleSquare() const {
    // returns a vector to a random point in the unit square with the centre at
    // the origin
    return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
  }

  Colour rayColour(const Ray &r, int depth, const Hittable &world) const {
    // if we hit the max depth, no more light will be gathered
    if (depth <= 0) {
      return Colour(0, 0, 0);
    }

    HitRecord rec;
    if (world.hit(r, Interval(0, infinity), rec)) {
      Vec3 direction = randomOnHemisphere(rec.normal);
      return 0.5 *
             rayColour(Ray(rec.p, direction), depth - 1,
                       world); // the colour is 50% of the colour from the bound
    }

    Vec3 unitDirection = unitVector(r.direction());
    auto alpha = 0.5 * (unitDirection.y() + 1.0); // puts alpha between 0 and 1
    return (1.0 - alpha) * Colour(1.0, 1.0, 1.0) +
           alpha * Colour(0.5, 0.7,
                          1.0); // linear interpolation between blue and white
  }
};

#endif