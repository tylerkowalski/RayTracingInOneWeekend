#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittableList.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
  // world
  HittableList world;

  auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  auto material_center = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
  auto material_left = make_shared<Metal>(Colour(0.8, 0.8, 0.8), 0.2);
  auto material_right = make_shared<Metal>(Colour(0.1, 0.1, 0.1), 1.0);

  world.add(
      make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
  // world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5,
  // material_center));
  world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
  world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_center));

  Camera cam;
  cam.ASPECT_RATIO = 16.0 / 9.0;
  cam.IMAGE_WIDTH = 400;
  cam.SAMPLES_PER_PIXEL = 100;
  cam.MAX_DEPTH = 50;

  cam.render(world);
}