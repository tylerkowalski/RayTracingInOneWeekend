#include "rtweekend.hpp"

#include "camera.hpp"
#include "hittable.hpp"
#include "hittableList.hpp"
#include "material.hpp"
#include "sphere.hpp"

int main() {
  // world
  HittableList world;

  //  auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  //  auto material_center = make_shared<Dielectric>(1.5);
  //  auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
  //  auto material_left = make_shared<Metal>(Colour(0.5, 0.5, 0.5), 0.1);
  //  auto material_right = make_shared<Metal>(Colour(0.1, 0.1, 0.1), 1.0);
  //  world.add(
  //      make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0,
  //      material_ground));
  //
  //  world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5,
  //  material_center));
  //  // world.add(make_shared<Sphere>(Point3(0, 0, -1.2), 0.4,
  //  material_bubble)); world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0),
  //  0.5, material_left)); world.add(make_shared<Sphere>(Point3(3.0, 0.0,
  //  -5.0), 1.0, material_right));

  //  auto R = cos(pi / 4);
  //
  //  auto matL = make_shared<Lambertian>(Colour(0, 0, 1));
  //  auto matR = make_shared<Lambertian>(Colour(1, 0, 0));
  //
  //  world.add(make_shared<Sphere>(Point3(-R, 0, -1), R, matL));
  //  world.add(make_shared<Sphere>(Point3(R, 0, -1), R, matR));
  // auto material_ground = make_shared<Lambertian>(Colour(0.8, 0.8, 0.0));
  // auto material_center = make_shared<Lambertian>(Colour(0.1, 0.2, 0.5));
  // auto material_left = make_shared<Dielectric>(1.50);
  // auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
  // auto material_right = make_shared<Metal>(Colour(0.8, 0.6, 0.2), 1.0);

  // world.add(
  //     make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0,
  //     material_ground));
  // world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5,
  // material_center)); world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0),
  // 0.5, material_left)); world.add(make_shared<Sphere>(Point3(-1.0, 0.0,
  // -1.0), 0.4, material_bubble)); world.add(make_shared<Sphere>(Point3(1.0,
  // 0.0, -1.0), 0.5, material_right));

  auto ground_material = make_shared<Lambertian>(Colour(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

  auto material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<Lambertian>(Colour(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<Metal>(Colour(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = randomDouble();
      Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

      if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = Colour::random() * Colour::random();
          sphere_material = make_shared<Lambertian>(albedo);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = Colour::random(0.5, 1);
          auto fuzz = randomDouble(0, 0.5);
          sphere_material = make_shared<Metal>(albedo, fuzz);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  Camera cam;
  cam.ASPECT_RATIO = 16.0 / 9.0;
  cam.IMAGE_WIDTH = 1200;
  cam.SAMPLES_PER_PIXEL = 100;
  cam.MAX_DEPTH = 50;

  cam.VFOV = 20;
  cam.lookfrom = Point3(13, 2, 3);
  cam.lookat = Point3(0, 0, -1);
  cam.vup = Vec3(0, 1, 0);

  cam.defocusAngle = 0.6;
  cam.focusDistance = 10.0;

  cam.render(world);
}