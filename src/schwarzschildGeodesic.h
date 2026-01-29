#ifndef SCHWARTZSCHILDGEODESIC
#define SCHWARTZSCHILDGEODESIC
#include "metric.h"
#include "observer.h"

struct RayResult {

  bool escaped;
  double chi;
};

class SchwarzschildGeodesic {
private:
  Metric *metric;

public:
  Observer observer;
  SchwarzschildGeodesic(Observer observer, Metric *metric)
      : observer(observer), metric(metric){};
  RayResult integrate(double delta, bool savePath = false) const;
};

#endif // !SCHWARTZCHILDGEODESIC
