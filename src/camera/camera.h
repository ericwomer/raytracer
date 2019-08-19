#if !defined(CAMERA_H)
#define CAMERA_H

#include "rendering/ray.h"

inline Vec3 random_in_unit_disk()
{
    Vec3 p;
    do {
        p = 2.0 * Vec3(double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, 0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}

class camera {
public:
    camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, double vfow, double aspect, double aperture, double focus_dist, double t0, double t1)
    {  // vfow is top to bottom in degrees
        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;
        double theta = vfow * M_PI / 180;
        double half_height = tan(theta / 2);
        double half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        verticle = 2 * half_height * focus_dist * v;
        // origin = Vec3(0.0, 0.0, 0.0);
    }

    inline ray get_ray(double s, double t)
    {
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x() + v * rd.y();
        double time = time0 + (double(rand()) / RAND_MAX) * (time1 - time0);
        return ray(origin + offset, lower_left_corner + s * horizontal + t * verticle - origin - offset, time);
    }

    Vec3   origin;
    Vec3   lower_left_corner;
    Vec3   horizontal;
    Vec3   verticle;
    Vec3   u, w, v;
    double time0, time1;
    double lens_radius;
};
#endif  // CAMERA_H