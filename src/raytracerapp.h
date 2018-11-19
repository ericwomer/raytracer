#if !defined(RAYTRACERAPP_H)
#define RAYTRACERAPP_H

#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <cstdint>

#include "base/base.h"
#include "camera/camera.h"
#include "materials/material.h"
#include "objects/hitable_list.h"
#include "objects/sphere.h"
#include "rendering/raytracer.h"
#include "system/file.h"
#include "system/system.h"

// Eric: Placeholder for now, don't pass this down to other functions yet

typedef struct OutFile_s {  // for a lack of a better name
    std::string name;
    Vec2<int>   resolution;
    int         samples;
} OutFile_t;

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

    virtual Version_t version() override { return versionNumber; }

private:
    std::vector<std::string> actions;
    raytracer                tracer;

    OutFile_t out_format;
};

#endif  // RAYTRACERAPP_H