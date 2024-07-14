#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "hittable.hpp"
#include "material.hpp"

class Sphere : public Hittable {
private:
  Point3 centre;
  double radius;
  shared_ptr<Material> mat;

public:
  Sphere(const Point3 &centre, double radius, shared_ptr<Material> mat)
      : centre{centre}, radius{radius}, mat{mat} {}

  bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override {
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
    if (!rayT.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!rayT.surrounds(root)) {
        return false;
      }
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outwardNormal = (rec.p - centre) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.mat = mat;
    return true;
  }
};

#endif