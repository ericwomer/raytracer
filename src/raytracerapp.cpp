#include <iostream>
#include <limits>
#include <string>
#include <tbb/tbb.h>

#include "raytracerapp.h"
std::vector<hitable*> random_scene()
{
    int n = 50000;
    std::vector<hitable*> list;
    list.push_back(new sphere(Vec3(0, -1000,0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for(int a = -10; a < 10; a++) {
        for(int b = -10; b < 10; b++) {
            double choose_mat = double(rand()) / RAND_MAX;
            Vec3 center(a+0.9*(double(rand())/RAND_MAX), 0.2, b+0.9*(double(rand())/ RAND_MAX));
            if((center-Vec3(4, 0.2,0)).length() > 0.9) {
                if(choose_mat < 0.8) { //diffuse
                    list.push_back(new moving_sphere(center, center+Vec3(0,0.5*( double(rand())/RAND_MAX), 0), 0.0, 1.0, 0.2,
                            new Lambertian( Vec3((double(rand())/ RAND_MAX) * (double(rand())/ RAND_MAX), (double(rand())/ RAND_MAX) * (double(rand())/ RAND_MAX), (double(rand())/ RAND_MAX) * (double(rand())/ RAND_MAX)))));
                } else if (choose_mat < 0.95) { // metal
                    list.push_back(new sphere(center, 0.2,
                            new Metal(Vec3(0.5*(1 + (double(rand()) / RAND_MAX)),0.5*(1 + (double(rand()) / RAND_MAX)), 0.5*(1 + (double(rand()) / RAND_MAX))), 0.5*(1 + (double(rand()) / RAND_MAX)))));
                } else { // glass
                    list.push_back(new sphere(center, 0.2, new Dielectric(1.5)));
                }
            }
        }
    }
    list.push_back(new sphere(Vec3(0, 1,0), 1.0, new Dielectric(1.5)));
    list.push_back(new sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1))));
    list.push_back(new sphere(Vec3(4,1,0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0)));
    std::cout << "list.size(): " << list.size() << "\n";
    return list;
}

RaytracingApp::RaytracingApp()
{
    app_name = project.name;
    version_number = project.version_number;
    project.version_number;

    out_format.name = "default";  // fix it so the extension is selected by the output format.
    out_format.ext = ".tga";

    render_parameters.resolution.e[0] = 640;
    render_parameters.resolution.e[1] = 480;
    render_parameters.samples = 10;
    render_parameters.threaded = true;
    render_parameters.grains = 50000;
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int RaytracingApp::main()
{
    // Do threading, brake up the view port and split off threads based on that
    std::vector<Pixel_t> pixels;  // lower case this

    // Convert this list to a class scene::objects or something
    // render based on is_renderable_in_scene bool
    std::vector<hitable*> list;  // [4];
    list.push_back(new sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5))));
    list.push_back(new sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5)));
    list.push_back(new sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1))));
    list.push_back(new sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.1, 0.1, 0.1), 0.0)));

    Vec3   lookfrom(13, 2, 3);
    Vec3   lookat(0, 0, 0);
    double dist_to_focus = 10;  // (lookfrom - lookat).length();
    double aperture = 0.0;

    camera cam(lookfrom, lookat, Vec3(0, 1, 0), 20, double(render_parameters.resolution.x()) / double(render_parameters.resolution.y()), aperture, dist_to_focus, 0.0, 1.0);
    pixels = tracer.render(&cam, Vec2<int>(render_parameters.resolution.x(), render_parameters.resolution.y()), render_parameters.samples, random_scene(), render_parameters.threaded,
                           render_parameters.grains);

    std::stringstream stream_name;
    stream_name << out_format.name << "-" << render_parameters.resolution.x() << "x" << render_parameters.resolution.y() << "-" << render_parameters.samples << out_format.ext;
    out_format.name = stream_name.str();
    std::cout << "name: " << out_format.name << " \nstream_name: " << stream_name.str() << "\n";
    // writeFile(out_format.name, Vec2<int>(render_parameters.resolution.x(), render_parameters.resolution.y()), pixels);

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

    // Set the internal application name as called from the terminal/console
    app_name = params[0].data();

    // and then strip the ./ or full path plus application (Windows) from the internal name to clean it up
#if defined(_WINDOWS)
    std::string::size_type n;
    n = app_name.rfind("\\");
    app_name = app_name.substr(n);
    app_name.erase(0, 1);
#else
    app_name.erase(0, 2);
#endif

    // Since its no longer needed dump the first parameter of params
    params.erase(params.begin());

    // Eric: !!! REWRITE Fix this slow mess, have it processed in one section.
    // Switch throughout the parameters.
    for (std::vector<std::string>::const_iterator i = params.begin(); i != params.end(); ++i) {
        if (*i == "--help" || *i == "-h") {
            help();
            return EXIT_SUCCESS;
        } else if (*i == "--version" || *i == "-V") {
            version();
            return EXIT_SUCCESS;
        } else if (*i == "--output" || *i == "-o" || *i == "--filename") {
            ++i;
            out_format.name = *i;  // Hopefully its a file and not another parameter
                                   // Eric: Fix later with a class/enum combo for checking
                                   // to see if its another parameter or not.
        } else if (*i == "--samples" || *i == "-s") {
            ++i;
            // test to see if the string contains numbers only
            if (i->find_first_not_of("0123456789") == std::string::npos) {
                render_parameters.samples = std::stoi(*i);
            } else {
                std::cout << "Error: Only numbers are valid option for --grains or -g in the range of 1 and " << std::numeric_limits<unsigned int>::max()
                          << "\n";
                return EXIT_FAILURE;
            }
        } else if (*i == "--width" || *i == "-x") {
            ++i;
            render_parameters.resolution.e[0] = std::stoi(*i);
        } else if (*i == "--height" || *i == "-y") {
            ++i;
            render_parameters.resolution.e[1] = std::stoi(*i);
        } else if (*i == "--threaded" || *i == "-t") {
            ++i;
            std::string result;
            for (auto c : *i) {
                result += std::tolower(c);
            }

            if (result == "true") {
                render_parameters.threaded = true;
            } else if (result == "false") {
                render_parameters.threaded = false;
            } else {
                std::cout << "Unknown value " << result << " for --threaded or -t, must be in the form of 'true' or 'false'.\n";
                return EXIT_FAILURE;
            }
        } else if (*i == "--grains" || *i == "-g") {
            ++i;

            // test to see if the string contains numbers only
            if (i->find_first_not_of("0123456789") == std::string::npos) {
                render_parameters.grains = std::stoi(*i);
            } else {
                std::cout << "Error: Only numbers are valid option for --grains or -g in the range of 1 and " << std::numeric_limits<unsigned int>::max()
                          << "\n";
                return EXIT_FAILURE;
            }

        } else {  // catch all to make sure there are no invalid parameters
            std::cout << "Error: Use of unknown parameter " << *i << "\n";
            std::cout << "type " << app_name << "--help|-h to see usage\n\n";
            return EXIT_FAILURE;
            dump.push_back(*i);
        }
    }

    return main();
}

void RaytracingApp::help(void)
{
    version();
    std::cout << "Usage: " << app_name << " [options] [output file options] [rendering options]...\n";
    std::cout << "\nInformation: \n";
    std::cout << " -h, --help \t\t Print this help message and exit.\n";
    std::cout << " -V, --version \t\t Print the version and exit.\n";
    std::cout << "\nOptions: \n";
    std::cout << " -o, --output \t\t Output file name.\n";
    std::cout << " -s, --samples \t\t Number of samples taken of each pixel. \n";
    std::cout << " -x, --width \t\t Image width. \n";
    std::cout << " -y, --height \t\t Image height. \n";
    std::cout << " -f, --format \t\t Output format [future use] \n";
    std::cout << " -t, --threaded \t Render using threading \n";
    std::cout << " -g, --grains \t\t If threaded, what number of grains to use. \n";
}