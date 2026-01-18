#ifndef METRIC
#define METRIC

class Metric {
  virtual ~Metric() = default;
  virtual double dot();
}

#endif // !METRIC
