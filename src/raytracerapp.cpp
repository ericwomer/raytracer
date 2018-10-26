#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "raytracerapp.hpp"

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

raytracerapp::raytracerapp() {
	app_description.push_back(std::string("raytracerapp \n"));
	app_description.push_back(std::string("** nothing else follows ** \n"));
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int raytracerapp::main(void) {
	// Start Main Application Here.
	// writePlaylist();

	// Do threading, brake up the view port and split off threads based on that
	int nx = 640;
	int ny = 480;
	int ns = 50; // Bounces?

	std::stringstream ppm_file, ppm_file_01, ppm_file_02;
	std::vector<char> ppm_data;
	// Convert this list to a class scene::objects or something
	// render based on isrenderable_in_scene bool
	hitable *list[4];
	list[0] =
	    new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	list[1] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[2] =
	    new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[3] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.1, 0.1, 0.1), 0.0));

	vec3 lookfrom(13, 2, -10);
	vec3 lookat(0, 0, 0);
	double dist_to_focus = 10; // (lookfrom - lookat).length();
	double aperture = 0.0;

	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny),
	           aperture, dist_to_focus);

	pixelrgb *data = tracer.render(&cam, vec2<int>(nx, ny), ns, list, 4);
	ppm_file = tracer.renderPPM(&cam, vec2<int>(nx, ny), ns, list, 4);

	writeFile("scene.tga", vec2<int>(nx, ny), data);
	writeFilePPM("scene.ppm", ppm_file.str());
	return 0;
}

// This is the main that handles parameters
int raytracerapp::main(std::vector<std::string> &params) {

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
			          << std::endl; // create a version raytracerapp class??
			return 0;
		} else {
			dump.push_back(c);
		}
	}

	return main();
}

// This main converts c style parameters to c++ strings
// then passes it to main that handles the actual parametrs.
int raytracerapp::main(int argv, char *argc[]) {
	// Start here if there are params
	std::vector<std::string> params;

	setAppName(argc[0]);

	for (int i = 1; i != argv; ++i) {
		params.push_back(argc[i]);
	}

	return main(params);
}

void raytracerapp::help(void) {
	std::cout << "Usage: " << app_name << " [options] files...\n\n";
	std::cout << "Options: \n";
	std::cout
	    << " -h, --help \t\t Print this help message and exit the program.\n";
	std::cout << " -v, --verbose \t\t Print out all the valid command line "
	             "parameters\n";
	std::cout << " \t\t\t passed to the program.\n";
	std::cout << " -V, --version \t\t Print the version and exit.\n";
}
