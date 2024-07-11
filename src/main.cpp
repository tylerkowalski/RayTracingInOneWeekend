#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittableList.hpp"
#include "sphere.hpp"

int main() {
  // world
  HittableList world;

  world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
  world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

  Camera cam;
  cam.ASPECT_RATIO = 16.0 / 9.0;
  cam.IMAGE_WIDTH = 400;

  cam.render(world);
}