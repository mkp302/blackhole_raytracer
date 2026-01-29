#ifndef OBSERVER
#define OBSERVER
#include "tetrad.h"
#include "vec4.h"

class Observer {

public:
  Vec4 position;

  Tetrad tetrad;
  Observer(Vec4 position) : position(position){};
};

#endif
