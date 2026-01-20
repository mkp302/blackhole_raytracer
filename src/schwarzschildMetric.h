#ifndef SCHWARZSCHILDMETRIC
#define SCHWARZSCHILDMETRIC

#include "metric.h"
#include "tetrad.h"
#include "vec4.h"

class SchwarzschildMetric : public Metric {
public:
  double M;

  explicit SchwarzschildMetric(double mass) : M(mass) {}
  Tetrad reference_tetrad(const Vec4 &position) const;
  double dot(const Vec4 &a, const Vec4 &b, const Vec4 &x) const;
};

#endif // !DEBUG
