#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "camera/camera.h"
#include "materials/material.h"
#include "objects/sphere.h"
#include "ray_engine/hitable_list.h"

#include "raytracer.h"

// Move this eventually to types
vec3 color(const ray &r, hitable *world, int depth) {
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

hitable *random_scene() {
	int n = 500;
	hitable **list = new hitable *[n + 1];
	list[0] =
	    new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choosen_mat = drand48();
			vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choosen_mat < 0.8) {
					list[i++] = new sphere(
					    center, 0.2,
					    new lambertian(vec3(drand48() * drand48(), drand48() * drand48(),
					                        drand48() * drand48())));
				} else if (choosen_mat < 0.95) {
					list[i++] = new sphere(
					    center, 0.2,
					    new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()),
					                   0.5 * (1 + drand48())),
					              0.5 * drand48()));
				} else {
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}
	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] =
	    new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] =
	    new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.0), 0.0));

	return new hitable_list(list, i);
}

int writeFile(std::string const &filename, std::string const &contents) {
	std::ofstream ofile;

	{

		// Close it incase it was left open.
		if (ofile.is_open()) {
			ofile.close();
		}
		ofile.open(filename);

		if (!ofile) {
			std::cerr << "Could not open " << filename << "\n";
			return -1;
		}

		ofile << contents;
		ofile.close();
	}

	return 0;
}

raytracer::raytracer() {
	app_description.push_back(std::string("Raytracer \n"));
	app_description.push_back(std::string("** nothing else follows ** \n"));
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int raytracer::main(void) {
	// Start Main Application Here.
	// writePlaylist();

	// Do threading, brake up the view port and split off threads based on that
	int nx = 800;
	int ny = 400;
	int ns = 50; // Bounces?

	std::stringstream ppm_file;

	std::cout << "Rendering\n" << std::endl;
	ppm_file << "P3\n" << nx << " " << ny << "\n255\n";

	// Convert this list to a class scene::objects or something
	// render based on isrenderable_in_scene bool
	hitable *list[4];
	list[0] =
	    new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	list[1] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[2] =
	    new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[3] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.1, 0.1, 0.1), 0.0));
	hitable *world = new hitable_list(list, 4);

	vec3 lookfrom(13, 2, -10);
	vec3 lookat(0, 0, 0);
	double dist_to_focus = 10; // (lookfrom - lookat).length();
	double aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny),
	           aperture, dist_to_focus);

	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				double u = double(i + drand48()) / double(nx);
				double v = double(j + drand48()) / double(ny);
				ray r = cam.get_ray(u, v);
				// vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			col /= double(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);

			ppm_file << ir << " " << ig << " " << ib << "\n";
		}
	}
	return writeFile("scene.ppm", ppm_file.str());
}

// This is the main that handles parameters
int raytracer::main(std::vector<std::string> &params) {

	// std::vector<std::string> actions;
	std::vector<std::string> dump;

	// iterate through params to remove the -- from the text
	for (std::vector<std::string>::const_iterator i = params.begin();
	     i != params.end(); ++i) {

		if (*i == "--help" || *i == "-h") {
			actions.push_back("help");
		} else if (*i == "--verbose" || *i == "-v") {
			actions.push_back("verbose");
		} else if (*i == "--version" || *i == "-V") {
			actions.push_back("version");
		} else { // catch all to make sure there are no invalid parameters
			dump.push_back(*i);
		}
	}

	for (auto &c : actions) { // handle all the prameters
		if (c == "help") {
			help();
			return 0; // exit the app

		} else if (c == "verbose") {

			for (auto &d : actions) {
				std::cout << "--" << d << " ";
			}
			std::cout << std::endl;
			return 0;
		} else if (c == "version") {
			std::cout << app_name << " " << 00 << "." << 00 << "." << 01
			          << std::endl; // create a version raytracer class??
			return 0;
		} else {
			dump.push_back(c);
		}
	}

	return main();
}

// This main converts c style parameters to c++ strings
// then passes it to main that handles the actual parametrs.
int raytracer::main(int argv, char *argc[]) {
	// Start here if there are params
	std::vector<std::string> params;

	setAppName(argc[0]);

	for (int i = 1; i != argv; ++i) {
		params.push_back(argc[i]);
	}

	return main(params);
}

void raytracer::help(void) {
	std::cout << "Usage: " << app_name << " [options] files...\n\n";
	std::cout << "Options: \n";
	std::cout
	    << " -h, --help \t\t Print this help message and exit the program.\n";
	std::cout << " -v, --verbose \t\t Print out all the valid command line "
	             "parameters\n";
	std::cout << " \t\t\t passed to the program.\n";
	std::cout << " -V, --version \t\t Print the version and exit.\n";
}
