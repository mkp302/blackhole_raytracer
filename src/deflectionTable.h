#ifndef DEFLECTIONTABLE
#define DEFLECTIONTABLE

#include <string>
#include <vector>
struct Deflection {
  double delta;
  double chi;
};

class DeflectionTable {
public:
  double deltaMin = 0;
  std::vector<Deflection> table;
  DeflectionTable(std::vector<Deflection> table) : table(table){};
  double sample(double delta) const;
  void dumpTable(std::string fileName);
};

#endif // !DEFLECTIONTABLE
