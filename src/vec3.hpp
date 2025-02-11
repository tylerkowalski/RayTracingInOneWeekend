#ifndef VEC3_HPP
#define VEC3_HPP

#include <ostream>

class Vec3 {
public:
  double e[3];

  Vec3() : e{0, 0, 0} {}
  Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  Vec3 &operator+=(const Vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  Vec3 &operator*=(double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  Vec3 &operator/=(double t) { return *this *= 1 / t; }

  double lengthSquared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

  bool nearZero() const {
    auto s = 1e-8;
    return (fabs(e[0] < s)) && (fabs(e[1] < s)) && (fabs(e[2]) < s);
  }

  double length() const { return sqrt(lengthSquared()); }

  static Vec3 random() {
    return Vec3(randomDouble(), randomDouble(), randomDouble());
  }

  static Vec3 random(double min, double max) {
    return Vec3(randomDouble(min, max), randomDouble(min, max),
                randomDouble(min, max));
  }
};

// probably not the best for type-safety, but we will use it for geometric
// clarity
using Point3 = Vec3;

// vector utility functions
inline std::ostream &operator<<(std::ostream &out, const Vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3 &v) {
  return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, double t) { return t * v; }

inline Vec3 operator/(const Vec3 &v, double t) { return (1 / t) * v; }

inline double dot(const Vec3 &u, const Vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unitVector(const Vec3 &v) { return v / v.length(); }

inline Vec3 randomInUnitDisk() {
  while (true) {
    auto p = Vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
    if (p.lengthSquared() < 1)
      return p;
  }
}

inline Vec3 randomInUnitSphere() {
  while (true) {
    auto p = Vec3::random(-1, 1); // sample a random point in the unit cube
    if (p.lengthSquared() <
        1) // it is IN the unit sphere but NOT necessarily on it
      return p;
  }
}

// note that any random unit vector is on the unit sphere
inline Vec3 randomUnitVector() { return unitVector(randomInUnitSphere()); }

inline Vec3 randomOnHemisphere(const Vec3 &normal) {
  Vec3 onUnitSphere = randomUnitVector();
  if (dot(onUnitSphere, normal) > 0) {
    return onUnitSphere;
  } else {
    return -onUnitSphere;
  }
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n) {
  // note, v+2b is the reflected ray, where b is -proj_n(v)
  // b = -proj_n(v) = [dot(v,n)/dot(n,n)]n, and n is a unit vector
  return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double etaiOverEtat) {
  auto cosTheta = fmin(
      dot(-uv, n), 1.0); // derived from the fact that uv, n are unit
                         // vectors and dot(a,b)=|a||b|cos(theta), i.e |a||b|=1
  Vec3 rOutPerp =
      etaiOverEtat *
      (uv + cosTheta * n); // etai is incoming eta (index of refraction)
  Vec3 rOutParallel = -sqrt(fabs(1.0 - rOutPerp.lengthSquared())) * n;
  // R_perp = etai/etat (R+cos(theta)N)
  // R_para = -sqrt(1-|R_perp|^2)N
  // the above come from Snell's law

  return rOutPerp + rOutParallel;
}

#endif