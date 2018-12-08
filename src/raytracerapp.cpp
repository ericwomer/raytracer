#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <sstream>
#include <tbb/tbb.h>

#include "system/window/window.h"
#include "raytracerapp.h"
#include "types/types.h"

template <typename T>
std::unique_ptr<T> generate_unique_ptr()
{
  auto temp = std::make_unique<T>();
  return temp;
}

RaytracingApp::RaytracingApp()
{
  app_name       = project.name;
  version_number = project.version_number;
  project.version_number;

  out_format.resolution.e[0] = 640;
  out_format.resolution.e[1] = 480;
  out_format.samples         = 10;
  out_format.name            = "default";  // fix it so the extension is selected by the output format.
  out_format.ext             = ".tga";
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

  Vec3   lookfrom(13, 2, -10);
  Vec3   lookat(0, 0, 0);
  double dist_to_focus = 10;  // (lookfrom - lookat).length();
  double aperture      = 0.0;

  camera cam(lookfrom,
             lookat,
             Vec3(0, 1, 0),
             20,
             double(out_format.resolution.x()) / double(out_format.resolution.y()),
             aperture,
             dist_to_focus);
  pixels =
      tracer.render(&cam, Vec2<int>(out_format.resolution.x(), out_format.resolution.y()), out_format.samples, list);

  std::stringstream stream_name;
  stream_name << out_format.name << "-" << out_format.resolution.x() << "x" << out_format.resolution.y() << "-"
              << out_format.samples << out_format.ext;
  out_format.name = stream_name.str();
  std::cout << "name: " << out_format.name << " \nstream_name: " << stream_name.str() << "\n";
  writeFile(out_format.name, Vec2<int>(out_format.resolution.x(), out_format.resolution.y()), pixels);

  return EXIT_SUCCESS;
}

// This is the main that handles parameters
int RaytracingApp::main(std::vector<std::string>& params)
{
  tbb::task_scheduler_init init(tbb::task_scheduler_init::automatic);

  /* std::cout << "Task Scheduler Active: " << init.is_active() << "\n";
    std::cout << "threads: " << init.default_num_threads() << "\n";
*/
  // std::vector<std::string> actions;
  std::vector<std::string> dump;

  app_name = params[0].data();
  app_name.erase(0, 2);

  // Eric: !!! REWRITE Fix this slow mess, have it processed in one section.
  // Switch throught the parameters.
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
      out_format.samples = std::stoi(*i);
    } else if (*i == "--width" || *i == "-x") {
      ++i;
      out_format.resolution.e[0] = std::stoi(*i);
    } else if (*i == "--height" || *i == "-y") {
      ++i;
      out_format.resolution.e[1] = std::stoi(*i);
    } else {  // catch all to make sure there are no invalid parameters
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
  std::cout << " -s, --samples \t\t Number of samples taken of each pixel. (?)\n";
  std::cout << " -x, --width \t\t Image width. \n";
  std::cout << " -y, --height \t\t Image height. \n";
  std::cout << " -f, --format \t\t Output format [future use] \n";
}
