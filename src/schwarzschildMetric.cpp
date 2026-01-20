#include "schwarzschildMetric.h"
#include "tetrad.h"
#include <cmath>

Tetrad SchwarzschildMetric::reference_tetrad(const Vec4 &position) const {
  Tetrad t;
  const double r = position.r;
  const double theta = position.theta;
  const double f = 1.0 - 2.0 * this->M / r;

  t.e0 = {1 / std::sqrt(f), 0.0, 0.0, 0.0};

  t.e1 = {0.0, std::sqrt(f), 0.0, 0.0};

  t.e2 = {0.0, 0.0, 1 / r, 0.0};

  t.e3 = {0.0, 0.0, 0.0, 1.0 / (r * std::sin(theta))};

  return t;
};

double SchwarzschildMetric::dot(const Vec4 &a, const Vec4 &b,
                                const Vec4 &x) const {
  return 0.;
};
