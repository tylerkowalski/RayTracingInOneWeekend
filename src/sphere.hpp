#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"

class Sphere : public Hittable {
private:
  Point3 centre;
  double radius;

public:
  Sphere(const Point3 &centre, double radius)
      : centre{centre}, radius{radius} {}

  bool hit(const Ray &r, double rayTMin, double rayTMax,
           HitRecord &rec) const override {
    Vec3 oc = centre - r.origin();
    auto a = r.direction().lengthSquared();
    auto h = dot(r.direction(), oc);
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0) {
      return false;
    }

    auto sqrtd = sqrt(discriminant);

    // find the closest root that is within bounds
    auto root = (h - sqrtd) / a;
    if (root <= rayTMin || rayTMax <= root) {
      root = (h + sqrtd) / a;
      if (root <= rayTMin || rayTMax <= root) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outwardNormal = (rec.p - centre) / radius;
    rec.setFaceNormal(r, outwardNormal);
    return true;
  }
};

#endif