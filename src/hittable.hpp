#ifndef HITTABLE_HPP
#define HITTABLE_HPP

class HitRecord {
public:
  Point3 p;
  Vec3 normal;
  double t;
  bool frontFace;

  void setFaceNormal(const Ray &r, const Vec3 &outwardNormal) {
    // sets the normal vector to always be against the incident ray
    // ASSUMED: outwardNormal is a unit vector

    frontFace = dot(r.direction(), outwardNormal) < 0;
    normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

// abstract base class
class Hittable {
public:
  virtual ~Hittable() = default;

  virtual bool hit(const Ray &r, double rayTMin, double rayTMax,
                   HitRecord &rec) const = 0;
};

#endif