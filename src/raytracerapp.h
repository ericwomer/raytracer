#if !defined(RAYTRACERAPP_H)
#define RAYTRACERAPP_H

#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <cstdint>

#include <functional>
#include <initializer_list>

#include "base/base.h"
#include "camera/camera.h"
#include "materials/material.h"
#include "objects/hitable_list.h"
#include "objects/sphere.h"
#include "rendering/raytracer.h"
#include "system/file.h"
#include "system/system.h"

#include "config.h"

// Eric: Parameters construction
/*
 * [struct/class] [name] = {
    [short param], [long param], [seperator aka = or ' '], [variable[list]], [function pointer[fp parameter list]]
 }
 *                      string  string  char/string string void*(?)    some_parameter_list
 * Parameter param = { "-h", "--help", " ", " ", Class::method, nullptr,
 *  "-o", "--output", " ", OutFile_t.name, RaytracingApp::help, nullptr} 
 */

// Eric: Placeholder for now, don't pass this down to other functions yet
typedef struct OutFile_s {  // for a lack of a better name
    std::string  name;
    std::string  ext;
    Vec2<int>    resolution;
    unsigned int samples;
} OutFile_t;

typedef struct Render_s {
    std::size_t grains;
    bool        threaded;
} Render_t;

class RaytracingApp : public Base {
public:
    RaytracingApp();

    virtual ~RaytracingApp(){};
    virtual int main() override;
    virtual int main(std::vector<std::string>& params) override;

    virtual int size() override { return sizeof(this); };

    virtual const std::string name() override { return app_name; }

    virtual void name(std::string name) override { app_name = name; }
    virtual void help() override;

    virtual void version() override
    {
        std::cout << project.name << " version " << project.version_number << "\n";
        std::cout << "Compiler: " << project.compiler_name << " " << project.compiler_version << "\n";
        std::cout << "Operating System: " << project.operating_system << "\n";
        std::cout << "Architecture: " << project.cpu_family << "\n" << std::endl;
    }

private:
    std::vector<std::string> actions;

    raytracer tracer;

    OutFile_t out_format;
    Render_t  render_parameters;
    Project_t project;
};

#endif  // RAYTRACERAPP_H