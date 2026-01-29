#ifndef SCHWARZSCHILDMETRIC
#define SCHWARZSCHILDMETRIC

#include "metric.h"
#include "tetrad.h"
#include "vec4.h"

class SchwartzschildMetric : public Metric {
public:
  double M;

  SchwartzschildMetric(double M) : Metric(M){};
  Tetrad reference_tetrad(const Vec4 &position) const;
  double dot(const Vec4 &a, const Vec4 &b, const Vec4 &x) const;
};

#endif // !DEBUG
