#if !defined(__SPHERE_H__)
#define __SPHERE_H__

#include "rendering/hitable.hpp"

class material;

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, double r, material *m)
	    : center(cen), radius(r), mat_ptr(m){};
	virtual bool hit(const ray &r, double t_min, double t_max,
	                 hit_record &rec) const;
	vec3 center;
	double radius;
	material *mat_ptr;
};

#endif // __SPHERE_H__