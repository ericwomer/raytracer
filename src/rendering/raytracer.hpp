#if !defined(__RAYTRACER_HPP__)
#define __RAYTRACER_HPP__

#include <iostream>
#include <sstream>

#include "camera/camera.hpp"
#include "materials/material.hpp"
#include "objects/sphere.hpp"
#include "rendering/hitable_list.hpp"
#include "types/vec2.hpp"
#include "types/vec3.hpp"

// Move this eventually to types
inline vec3 color(const ray &r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, std::numeric_limits<double>::max(), rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		} else {
			return vec3(0, 0, 0);
		}
	} else {
		vec3 unit_direction = unit_vector(r.direction());
		double t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

class raytracer {
public:
	raytracer(){};
	std::stringstream render(camera *cam, vec2<int> res, int samples,
	                         hitable *elements[], int element_count);
};

#endif // __RAYTRACER_HPP__