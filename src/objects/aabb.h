//
// Created by rake on 8/21/19.
//

#ifndef RAYTRACE_AABB_H
#define RAYTRACE_AABB_H

#include "types/vec3.h"
#include "rendering/ray.h"

inline double ffmin(double a, double b) { return a < b ? a : b;}
inline double ffmax(double a, double b) { return a > b ? a : b;}

class aabb {
public:
    aabb() {}
    aabb(const Vec3& a , const Vec3& b) : _min(a), _max(b) {}

    Vec3 min() const { return _min; }
    Vec3 max() const { return _max; }

    bool hit(const ray& r, double tmin, double tmax) const
    {
        for (int a = 0; a < 3; a++) {
            double t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);
            double t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a], (_max[a] - r.origin()[a]) / r.direction()[a]);

            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);
            if(tmax <= tmin) {
                return false;
            }
        }
        return true;
    }

    Vec3 _min;
    Vec3 _max;

};

aabb surrounding_box(aabb box0, aabb box1);

#endif //RAYTRACE_AABB_H
