
#include "system.hpp"

inline vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}