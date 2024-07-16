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
  Metal(const Colour &albedo, double fuzz)
      : albedo{albedo}, fuzz{fuzz < 1 ? fuzz : 1} {}

  bool scatter(const Ray &rIn, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {

    Vec3 reflected = reflect(rIn.direction(), rec.normal);
    reflected =
        unitVector(reflected) + (fuzz * randomUnitVector()); // fuzzing things
    // reflected ray has to first be normalized so that it is consistently
    // scaled WRT. fuzz sphere (otherwise fuzz factor is meaningless)

    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return dot(scattered.direction(), rec.normal) >
           0; // if our fuzzing causes the scatter to be towards the surface, we
              // absorb the ray
  }

private:
  Colour albedo;
  double fuzz; // the scaling factor of the fuzz unit sphere radius
};

class Dialectric : public Material {
public:
  Dialectric(double refractionIndex) : refractionIndex{refractionIndex} {}

  // always refracts
  bool scatter(const Ray &rIn, const HitRecord &rec, Colour &attenuation,
               Ray &scattered) const override {
    attenuation = Colour(1.0, 1.0, 1.0);
    double ri =
        rec.frontFace
            ? (1.0 / refractionIndex)
            : refractionIndex; // numerator in refractionIndex ratio needs to be
                               // refractive index of incoming material

    Vec3 unitDirection = unitVector(rIn.direction());
    double cosTheta =
        fmin(dot(-unitDirection, rec.normal),
             1.0); // cos(theta) between 2 unit vector are their dot product
    double sinTheta =
        sqrt(1.0 - cosTheta * cosTheta); // follows from pythagorean identity

    bool cannotRefract =
        ri * sinTheta > 1.0; // if there is no solution to Snell's law
    Vec3 direction;

    if (cannotRefract || reflectance(cosTheta, ri) > randomDouble())
      direction = reflect(unitDirection, rec.normal);
    else
      direction = refract(unitDirection, rec.normal, ri);

    scattered = Ray(rec.p, direction);

    return true;
  }

private:
  double
      refractionIndex; // ratio of material's index / index of enclosing media

  // reflectance varies with angle
  static double reflectance(double cosine, double refractionIndex) {
    // use the Schlick Approximation
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};

#endif