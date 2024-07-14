#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "rtweekend.hpp"

class HitRecord;

class Material {
public:
  virtual ~Material() = default;

  // attenuation, scattered are modified parameters
  virtual bool scatter(const Ray &rIn, const HitRecord &rec,
                       Colour &attenuation, Ray &scattered) const {
    return false;
  }
};

class Lambertian : public Material {
public:
  Lambertian(const Colour &albedo) : albedo{albedo} {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {
    auto scatterDirection =
        rec.normal + randomUnitVector(); // lambertion reflection
    // if you sample randomly from the unit sphere which lies tagent to the
    // point of intersection, in the direction of the incoming ray's origin,
    // the corresponding ray direction from the point of intersection to the
    // sampled point follows a distribution proportional to cos(theta); theta
    // is angle between the norm and the incoming ray. This is because you are
    // projecting sampled points from a unit sphere to a hemisphere, and more
    // points near the normal correspond to a projection from a point sampled
    // off the unit sphere. Equidistant points on the sphere, further from the
    // normal, will correspond to further points on the projection onto the
    // hemisphere. Slight changes of point samples far from the norm on the
    // unit sphere correspond to larger changes in the hemisphere, as opposed
    // to points sampled near the origin. Easier to reason about in 2D

    // catch degenerate case where random unit vector is oppose of normal
    if (scatterDirection.nearZero())
      scatterDirection = rec.normal;

    scattered = Ray(rec.p, scatterDirection);
    attenuation = albedo;
    return true;
  }

private:
  Colour albedo;
};

class Metal : public Material {
public:
  Metal(const Colour &albedo) : albedo{albedo} {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {

    Vec3 reflected = reflect(rIn.direction(), rec.normal);
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return true;
  }

private:
  Colour albedo;
};

#endif