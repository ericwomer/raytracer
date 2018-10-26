
#include "raytracer.h"

std::stringstream raytracer::renderPPM(camera *cam, vec2<int> res, int samples,
                                       hitable *elements[], int element_count) {

	std::stringstream buf;
	buf << "P3\n" << res.x() << " " << res.y() << "\n255\n";

	hitable *world = new hitable_list(elements, element_count);

	for (int j = res.y() - 1; j >= 0; j--) {
		for (int i = 0; i < res.x(); i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < samples; s++) {
				double u = double(i + drand48()) / double(res.x());
				double v = double(j + drand48()) / double(res.y());
				ray r = cam->get_ray(u, v);
				// vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= double(samples);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			buf << ir << " " << ig << " " << ib << "\n";
		}
	}
	return buf;
}

pixelrgb_t *raytracer::render(camera *cam, vec2<int> res, int samples,
                              hitable *elements[], int element_count) {

	hitable *world = new hitable_list(elements, element_count);

	pixelrgb_t data[res.y()][res.x()];

	for (int j = res.y() - 1; j >= 0; j--) {
		for (int i = 0; i < res.x(); i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < samples; s++) {
				double u = double(i + drand48()) / double(res.x());
				double v = double(j + drand48()) / double(res.y());
				ray r = cam->get_ray(u, v);
				// vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= double(samples);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			data[j][i].r = ir;
			data[j][i].g = ig;
			data[j][i].b = ib;
		}
	}
	return *data;
}