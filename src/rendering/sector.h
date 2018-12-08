#if !defined(SECTOR_H)
#define SECTOR_H

#include <sstream>
#include <vector>

#include "types/vec2.h"

// Used for
class sector {
  public:
  int x_start;
  int y_start;

  int x_end;
  int y_end;

  std::vector<std::vector<int>> section;
};

#endif  // SECTOR_H