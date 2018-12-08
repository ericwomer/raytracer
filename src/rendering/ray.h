#if !defined(RAY_H)
#define RAY_H

#include "types/vec3.h"

class ray {
  public:
  ray() {}
  ray(const Vec3& a, const Vec3& b)
  {
    A = a;
    B = b;
  }

  Vec3 origin() const { return A; }
  Vec3 direction() const { return B; }
  Vec3 point_at_parameter(double t) const { return A + t * B; }

  Vec3 A;
  Vec3 B;
};

#endif  // RAY_H