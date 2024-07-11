#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"

#include <vector>

class HittableList : public Hittable {
public:
  std::vector<shared_ptr<Hittable>> objects;

  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }

  void clear() { objects.clear(); }

  void add(shared_ptr<Hittable> object) { objects.push_back(object); }

  bool hit(const Ray &r, Interval rayT, HitRecord &rec) const override {
    HitRecord tmpRec;
    bool hitAnything = false;
    auto closestSoFar = rayT.max;

    for (const auto &object : objects) {
      if (object->hit(r, Interval(rayT.min, closestSoFar), tmpRec)) {
        hitAnything = true;
        closestSoFar = tmpRec.t;
        rec = tmpRec;
      }
    }
    return hitAnything;
  }
};

#endif