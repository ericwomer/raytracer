#if !defined(__SECTOR_H__)
#define __SECTOR_H__

#include <sstream>
#include <vector>

#include "types/vec2.hpp"

// Used for
class sector {
public:
	int x_start;
	int y_start;

	int x_end;
	int y_end;

	std::vector<std::vector<int>> section;
};

#endif // __SECTOR_H__