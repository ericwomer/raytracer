#if !defined(HITABLE_H)
#define HITABLE_H

#include "rendering/ray.h"
#include "aabb.h"

class Material;

struct hit_record {
    double    t;
    Vec3      p;
    Vec3      normal;
    Material* mat_ptr;
    char*     name;
};

class hitable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double t0, double t1, aabb& box) const = 0;
};

#endif  // HITABLE_H