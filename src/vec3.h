#ifndef VEC3
#define VEC3

struct Vec3 {
  double x, y, z;
  Vec3 operator+(const Vec3 &o) const { return {x + o.x, y + o.y, z + o.z}; }

  Vec3 operator-(const Vec3 &o) const { return {x - o.x, y - o.y, z - o.z}; }

  Vec3 operator*(double s) const { return {s * x, s * y, s * z}; }

  Vec3 operator/(double s) const { return {x / s, y / s, z / s}; }
};
#endif
