
#include "raytracer.h"

std::stringstream raytracer::renderPPM(camera *cam, vec2<int> res, int samples,
                                       std::vector<hitable *> elements) {

	std::stringstream buf;
	buf << "P3\n" << res.x() << " " << res.y() << "\n255\n";

	hitable *world = new hitable_list(elements.data(), elements.size());

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

std::vector<pixel> raytracer::render(camera *cam, vec2<int> res, int samples,
                                     std::vector<hitable *> elements) {

	hitable *world = new hitable_list(elements.data(), elements.size());

	// pixelrgba_t data[res.y()][res.x()];
	std::vector<pixel> Pixels;
	Pixels.resize(res.x() * res.y());

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
			uint8_t ir = uint8_t(255.99 * col[0]);
			uint8_t ig = uint8_t(255.99 * col[1]);
			uint8_t ib = uint8_t(255.99 * col[2]);
			uint8_t ia = uint8_t(255.99 * 1);
			Pixels[(res.y() - 1 - j) * res.x() + i].pixelData.r =
			    ir; // data[j][i].r = ir;
			Pixels[(res.y() - 1 - j) * res.x() + i].pixelData.g =
			    ig; // data[j][i].g = ig;
			Pixels[(res.y() - 1 - j) * res.x() + i].pixelData.b =
			    ib; // data[j][i].b = ib;
			Pixels[(res.y() - 1 - j) * res.x() + i].pixelData.a =
			    ia; // data[j][i].a = 1;
		}
	}
	return Pixels;
}