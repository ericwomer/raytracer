#if !defined(__SECTOR_H__)
#define __SECTOR_H__

#include "types/vec2.h"

// Used for
class sector {
public:
	uint32_t position;
	uint32_t width;
	uint32_t height;
	vec2<uint32_t> top_left;
	vec2<uint32_t> top_right;
	vec2<uint32_t> bottom_left;
	vec2<uint32_t> bottom_right;

	vec2<double> real_top_left;
	vec2<double> real_top_right;
	vec2<double> real_bottom_left;
	vec2<double> real_bottom_right;
};

#endif // __SECTOR_H__