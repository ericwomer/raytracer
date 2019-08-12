#if !defined(SPHERE_H)
#define SPHERE_H

#include "objects/hitable.h"

class material;

class sphere : public hitable {
  public:
  sphere() {}
  sphere(Vec3 cen, double r, Material* m)
      : center(cen)
      , radius(r)
      , mat_ptr(m){};
  virtual ~sphere(){};
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

  Vec3      center;
  double    radius;
  Material* mat_ptr;
};

#endif  // SPHERE_H