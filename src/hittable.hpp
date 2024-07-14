#ifndef HITTABLE_HPP
#define HITTABLE_HPP

class Material;

class HitRecord {
public:
  Point3 p;
  Vec3 normal;
  shared_ptr<Material> mat; // call member functions of this to determine
                            // scattered ray + properties
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

  virtual bool hit(const Ray &r, Interval rayT, HitRecord &rec) const = 0;
};

#endif