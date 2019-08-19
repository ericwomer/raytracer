#if !defined(RAY_H)
#define RAY_H

#include "types/vec3.h"

class ray {
public:
    ray() {}
    ray(const Vec3& a, const Vec3& b, double ti = 0)
    {
        A = a;
        B = b;
        _time = ti;
    }

    Vec3 origin() const { return A; }
    Vec3 direction() const { return B; }
    Vec3 point_at_parameter(double t) const { return A + t * B; }
    double time() const { return _time; }

    Vec3 A;
    Vec3 B;
    double _time;
};

#endif  // RAY_H