#if !defined(HITABLE_H)
#define HITABLE_H

#include "ray.h"

class material;

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
	material *mat_ptr;
	char *name;
};

class hitable {
public:
	virtual ~hitable() {}
	virtual bool hit(const ray &r, double t_min, double t_max,
	                 hit_record &rec) const = 0;
};

#endif // HITABLE_H