#include "schwarzschildMetric.h"
#include "tetrad.h"
#include <cmath>

// Reference Tetrad for a static observer (for now )
// Paper references am observer in freefall will fix later
//
Tetrad SchwarzschildMetric::reference_tetrad(const Vec4 &position) const {
  Tetrad t;
  const double r = position.r;
  const double theta = position.theta;
  const double f = 1.0 - 2.0 * M / r;

  t.e0 = {1 / std::sqrt(f), -1 * sqrt(2 * M / r), 0.0, 0.0};

  t.e1 = {-1.0 * sqrt(2 * M / r) / f, 1.0, 0.0, 0.0};

  t.e2 = {0.0, 0.0, 1 / r, 0.0};

  t.e3 = {0.0, 0.0, 0.0, 1.0 / (r * std::sin(theta))};

  return t;
};

double SchwarzschildMetric::dot(const Vec4 &a, const Vec4 &b,
                                const Vec4 &x) const {
  const double r = x.r;
  const double theta = x.theta;
  const double A = 1.0 - 2.0 * M / r;

  return -A * a.t * b.t + (1.0 / A) * a.r * b.r + r * r * a.theta * b.theta +
         r * r * std::sin(theta) * std::sin(theta) * a.phi * b.phi;
};
