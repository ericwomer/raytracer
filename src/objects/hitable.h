#if !defined(HITABLE_H)
#define HITABLE_H

#include "rendering/ray.h"

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
  virtual ~hitable() {}
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif  // HITABLE_H