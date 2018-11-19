#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <tbb/tbb.h>

#include "raytracerapp.h"
#include "types/types.h"

RaytracingApp::RaytracingApp()
{
    app_description.push_back(std::string("Ray Tracing App \n"));
    app_description.push_back(std::string("** nothing else follows ** \n"));

    out_format.resolution.e[0] = 640;
    out_format.resolution.e[1] = 480;
    out_format.samples = 50;
    out_format.name = "out.tga";  // fix it so the extension is selected by the output format.
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int RaytracingApp::main()
{
    // Start Main Application Here.

    // Do threading, brake up the view port and split off threads based on that

    std::vector<Pixel_t> pixels;  // lower case this

    // Convert this list to a class scene::objects or something
    // render based on is_renderable_in_scene bool
    std::vector<hitable*> list;  // [4];
    list.push_back(new sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));
    list.push_back(new sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
    list.push_back(new sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1))));
    list.push_back(new sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.1, 0.1, 0.1), 0.0)));

    Vec3   lookfrom(13, 2, -10);
    Vec3   lookat(0, 0, 0);
    double dist_to_focus = 10;  // (lookfrom - lookat).length();
    double aperture = 0.0;

    camera cam(lookfrom, lookat, Vec3(0, 1, 0), 20, double(out_format.resolution.x()) / double(out_format.resolution.y()), aperture, dist_to_focus);
    pixels = tracer.render(&cam, Vec2<int>(out_format.resolution.x(), out_format.resolution.y()), out_format.samples, list);

    writeFile(out_format.name, Vec2<int>(out_format.resolution.x(), out_format.resolution.y()), pixels);

    return EXIT_SUCCESS;
}

// This is the main that handles parameters
int RaytracingApp::main(std::vector<std::string>& params)
{
    tbb::task_scheduler_init init;

    std::cout << "Task Scheduler Active: " << init.is_active() << "\n";
    std::cout << "threads: " << init.default_num_threads() << "\n";

    // std::vector<std::string> actions;
    std::vector<std::string> dump;

    // iterate through params to remove the -- from the text
    for (std::vector<std::string>::const_iterator i = params.begin(); i != params.end(); ++i) {
        if (*i == "--help" || *i == "-h") {
            actions.push_back("help");
        } else if (*i == "--verbose" || *i == "-v") {
            actions.push_back("verbose");
        } else if (*i == "--version" || *i == "-V") {
            actions.push_back("version");
        } else {  // catch all to make sure there are no invalid parameters
            dump.push_back(*i);
        }
    }

    for (auto& c : actions) {  // handle all the prameters
        if (c == "help") {
            help();
            return 0;  // exit the app
        } else if (c == "verbose") {
            for (auto& d : actions) {
                std::cout << "--" << d << " ";
            }
            std::cout << std::endl;
            return 0;
        } else if (c == "version") {
            std::cout << app_name << " " << 00 << "." << 00 << "." << 01 << std::endl;  // create a version raytracerapp class??
            return 0;
        } else {
            dump.push_back(c);
        }
    }

    return main();
}

void RaytracingApp::help(void)
{
    std::cout << "Usage: " << app_name << " [options] files...\n\n";
    std::cout << "Options: \n";
    std::cout << " -h, --help \t\t Print this help message and exit the program.\n";
    std::cout << " -v, --verbose \t\t Print out all the valid command line "
                 "parameters\n";
    std::cout << " \t\t\t passed to the program.\n";
    std::cout << " -V, --version \t\t Print the version and exit.\n";
}