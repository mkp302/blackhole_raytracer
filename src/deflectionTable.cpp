#include "deflectionTable.h"
#include <fstream>
#include <iostream>
#include <string>

void DeflectionTable::dumpTable(std::string fileName) {
  std::ofstream out(fileName);
  out << "delta,chi\n";

  for (const auto &s : this->table) {
    out << s.delta << "," << s.chi << "\n";
  }
  return;
}

double DeflectionTable::sample(double delta) const {

  if (delta <= table.front().delta)
    return this->table.front().chi;

  if (delta >= table.back().delta)
    return table.back().chi;

  auto it = std::lower_bound(
      this->table.begin(), this->table.end(), delta,
      [](const Deflection &s, double d) { return s.delta < d; });

  const Deflection &s1 = *(it - 1);
  const Deflection &s2 = *it;

  double t = (delta - s1.delta) / (s2.delta - s1.delta);
  return s1.chi + t * (s2.chi - s1.chi);
};
