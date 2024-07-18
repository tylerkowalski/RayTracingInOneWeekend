#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend.hpp"

#include "hittable.hpp"
#include "material.hpp"

class Camera {
public:
  // rather than using a constructor or setters, these 2 fields are public and
  // define how intialize() will set the other variables
  double ASPECT_RATIO = 1.0;  // default image width / image height
  int IMAGE_WIDTH = 100;      // default value for image width
  int SAMPLES_PER_PIXEL = 10; // number of RANDOM samples per pixel
  int MAX_DEPTH = 10;         // maximum number of ray bounces we will allow

  double VFOV = 90;                  // vertical FOV
  Point3 lookfrom = Point3(0, 0, 0); // camera centre
  Point3 lookat = Point3(0, 0, -1);  // point the camera is looking at
  Vec3 vup = Vec3(0, 1, 0);          // camera-relative "up" direction

  double defocusAngle =
      0; // angle of defocus cone with base at CAMERA_CENTRE
         // and vertex at centre of viewport (better than defining the height of
         // the cone because amount of defocus is still dependent on the
         // projection distance)
  double focusDistance = 10; // distance from LOOKFROM to perfect focus plane

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
  Vec3 u, v, w;      // camera frame orthonormal basis vectors. u(right), v(up),
                     // w(opposite view direction)
  Vec3 defocusDiskU; // horizontal radius of defocus disk
  Vec3 defocusDiskV; // vertical radius of defocus disk

  void initialize() {
    // calculate the image height with min val = 1
    IMAGE_HEIGHT = int(IMAGE_WIDTH / ASPECT_RATIO);
    IMAGE_HEIGHT = (IMAGE_HEIGHT < 1) ? 1 : IMAGE_HEIGHT;

    PIXEL_SAMPLES_SCALE = 1.0 / SAMPLES_PER_PIXEL;

    CAMERA_CENTRE = lookfrom;

    // determine viewport dimensions
    auto theta = degreesToRadians(VFOV);
    auto h = tan(theta / 2); // since we "pretend" use the z=-1 plane
    auto VIEWPORT_HEIGHT =
        2 * h * focusDistance; // by similar triangle, distY is scaled the same
                               // as dist-Z (the scale factor is focusDistance)
    auto VIEWPORT_WIDTH =
        VIEWPORT_HEIGHT * (double(IMAGE_WIDTH) / IMAGE_HEIGHT);

    w = unitVector(lookfrom - lookat);
    u = unitVector(
        cross(vup, w)); // pick the vec perp to w and vup to be (right) [ this
                        // is what captures vups notion of "upness"]
    // what this means is that moving in the u direction does not affect the
    // position's vup component or w component, making our camera
    // horizontally-level wrt up vector (i.e moving left and right doesnt cause
    // moveup in upness WRT vup)

    // u,w define the "forward, sideways" movement, and we want it to be such
    // that movement in those axis don't add components wrt vup vector
    // the we define v as needed to have an orthonormal basis for camera
    v = cross(w, u); // pick up vector to be perp to other vectors

    // vectors that define the viewport
    auto VIEWPORT_U = VIEWPORT_WIDTH * u;
    auto VIEWPORT_V = VIEWPORT_HEIGHT * -v; // vector needs to go "down"

    // pixel distance vectors
    PIXEL_DELTA_U = VIEWPORT_U / IMAGE_WIDTH;
    PIXEL_DELTA_V = VIEWPORT_V / IMAGE_HEIGHT;

    // position of upper-left of the viewport
    auto VIEWPORT_UPPER_LEFT = CAMERA_CENTRE - (focusDistance * w) -
                               (VIEWPORT_U / 2) - (VIEWPORT_V / 2);
    PIXEL00_LOC =
        VIEWPORT_UPPER_LEFT + (PIXEL_DELTA_U / 2) + (PIXEL_DELTA_V / 2);

    // calculate the basis vectors for the defocus disk
    auto defocusRadius =
        focusDistance * tan(degreesToRadians(defocusAngle / 2));
    defocusDiskU = u * defocusRadius;
    defocusDiskV = v * defocusRadius;
  }

  Ray getRay(int m, int n) const {
    // make a ray from defocus disk to a random sample in the region of pixel
    // (i,j)
    auto offset = sampleSquare();
    auto pixelSample = PIXEL00_LOC + (n + offset.x()) * PIXEL_DELTA_U +
                       (m + offset.y()) * PIXEL_DELTA_V;

    auto rayOrigin = (defocusAngle <= 0) ? CAMERA_CENTRE : defocusDiskSample();
    auto rayDirection = pixelSample - rayOrigin;

    return Ray(rayOrigin, rayDirection);
  }

  Vec3 sampleSquare() const {
    // returns a vector to a random point in the unit square with the centre at
    // the origin
    return Vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
  }

  Point3 defocusDiskSample() const {
    auto p = randomInUnitDisk();
    return CAMERA_CENTRE + (p[0] * defocusDiskU) + (p[1] * defocusDiskV);
  }

  Colour rayColour(const Ray &r, int depth, const Hittable &world) const {
    // if we hit the max depth, no more light will be gathered
    if (depth <= 0) {
      return Colour(0, 0, 0);
    }

    HitRecord rec;
    if (world.hit(r, Interval(0.001, infinity), rec)) {
      Ray scattered;
      Colour attenuation;
      if (rec.mat->scatter(r, rec, attenuation, scattered))
        return attenuation * rayColour(scattered, depth - 1, world);
      else {
        return Colour(0, 0, 0); // otherwise the ray is completely absorbed
      }
    }

    Vec3 unitDirection = unitVector(r.direction());
    auto alpha = 0.5 * (unitDirection.y() + 1.0); // puts alpha between 0 and 1
    return (1.0 - alpha) * Colour(1.0, 1.0, 1.0) +
           alpha * Colour(0.5, 0.7,
                          1.0); // linear interpolation between blue and white
  }
};

#endif