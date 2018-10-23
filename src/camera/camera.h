#if !defined(__CAMERA_H__)
#define __CAMERA_H__

#include "ray_engine/ray.h"

vec3 random_in_unit_disk() {
	vec3 p;
	do {
		p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfow, double aspect,
	       double aperture,
	       double focus_dist) { // vfow is top to bottom in degrees
		lens_radius = aperture / 2;
		double theta = vfow * M_PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - half_width * focus_dist * u -
		                    half_height * focus_dist * v - focus_dist * w;
		horizontal = 2 * half_width * focus_dist * u;
		verticle = 2 * half_height * focus_dist * v;
		// origin = vec3(0.0, 0.0, 0.0);
	}

	ray get_ray(double s, double t) {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lower_left_corner + s * horizontal +
		                                t * verticle - origin - offset);
	}

	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 verticle;
	vec3 u, w, v;
	double lens_radius;
};
#endif // __CAMERA_H__