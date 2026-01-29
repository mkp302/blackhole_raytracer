#ifndef METRIC
#define METRIC
#include "vec4.h"

class Metric {
public:
  double M;
  Metric(double M) : M(M){};
  virtual double dot(const Vec4 &a, const Vec4 &b, const Vec4 &x) const = 0;
  virtual ~Metric() = default;
};

#endif // !METRIC
