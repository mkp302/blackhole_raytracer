#ifndef VEC4
#define VEC4

struct Vec4 {
  double t, r, theta, phi;
  Vec4 operator+(const Vec4 &o) const {
    return {t + o.t, r + o.r, theta + o.theta, phi + o.phi};
  }

  Vec4 operator-(const Vec4 &o) const {
    return {t - o.t, r - o.r, theta - o.theta, phi - o.phi};
  }

  Vec4 operator*(double s) const { return {s * t, s * r, s * theta, s * phi}; }

  Vec4 operator/(double s) const { return {t / s, r / s, theta / s, phi / s}; }
};
#endif
