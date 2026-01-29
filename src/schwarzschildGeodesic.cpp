#include "schwartzschildGeodesic.h"
#include <array>
#include <boost/numeric/odeint.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

void save(std::string fileName, std::vector<std::pair<double, double>> path) {
  std::ofstream out(fileName);
  out << "x,y\n";

  for (const auto &s : path) {
    out << s.first << "," << s.second << "\n";
  }
  return;
}
using State = std::array<double, 8>;
enum { T = 0, R, TH, PH, KT, KR, KTH, KPH };

struct SchwarzschildGeodesicODE {
  double M;
  void operator()(const State &y, State &dydp, double /*p*/) const {
    double r = y[R];
    double phi = y[PH];
    double kt = y[KT], kr = y[KR], kphi = y[KPH];

    double A = 1.0 - 2 * M / r;
    double Ap = 2 * M / (r * r);

    // dx^mu/dp
    dydp[T] = kt;
    dydp[R] = kr;
    dydp[TH] = 0.0;
    dydp[PH] = kphi;

    // null geodesic
    dydp[KT] = -1 * Ap / A * kt * kr;
    dydp[KR] = -0.5 * A * Ap * kt * kt + 0.5 * (Ap / A) * kr * kr +
               A * r * kphi * kphi;
    dydp[KTH] = 0.0;
    dydp[KPH] = -2.0 / r * kr * kphi;
  }
};

RayResult SchwartzschildGeodesic::integrate(double delta, bool savePath) const {
  State y{};
  Tetrad t = observer.tetrad;
  std::vector<std::pair<double, double>> path;

  // Initial position
  y[T] = observer.position.t;
  y[R] = observer.position.r;
  y[TH] = observer.position.theta;
  y[PH] = observer.position.phi;

  // Initial wavevector in tetrad frame
  Vec4 k = t.e0 + t.e1 * std::cos(delta) + t.e3 * std::sin(delta);

  // Null normalization
  double norm = metric->dot(k, k, observer.position);

  // Map to coordinate components
  y[KT] = k.t;
  y[KR] = k.r;
  y[KPH] = k.phi;
  y[KTH] = M_PI / 2;

  SchwarzschildGeodesicODE ode{metric->M};
  using namespace boost::numeric::odeint;

  auto controlled = make_controlled<runge_kutta_cash_karp54<State>>(1e-9, 1e-9);

  double p = 0.0;
  double dp = -0.001; // affine step
  const double r_escape = 1e3 * metric->M;
  const double r_horizon = 2.0 * metric->M * (1.0 + 1e-6);

  for (int i = 0; i < 1000000; ++i) {
    controlled.try_step(ode, y, p, dp);
    if (savePath) {
      double r = y[R];
      double phi = y[PH];

      double x = r * std::cos(phi);
      double y2 = -1 * r * std::sin(phi);
      path.emplace_back(x, y2);
    }
    if (std::isnan(y[R])) {
      std::cerr << "NaN encountered at step " << i << "\n";
      return {false, 0.0};
    }

    // Escape / capture checks
    if (y[R] > r_escape) {
      if (savePath)
        save("path.csv", path);
      return {true, y[PH]}; // escaped
    }
    if (y[R] < 2.0 * metric->M * (1.0 + 1e-8)) {
      if (savePath)
        save("path.csv", path);

      return {false, 0.0};
    }
    // captured  }
  }
  std::cout << "Max steps exceeded " << y[R] << "\n";
  // Max steps exceeded
  return {false, y[PH]};
};
