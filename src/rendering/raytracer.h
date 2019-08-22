#if !defined(RAYTRACER_H)
#define RAYTRACER_H

#include <iostream>
#include <sstream>
#include <vector>

#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include "camera/camera.h"
#include "materials/material.h"
#include "objects/hitable_list.h"
#include "objects/sphere.h"
#include "types/types.h"
#include "types/vec2.h"
#include "types/vec3.h"

template <class T, class C>
class Pixel;

// determine the size of the pixel
class raytracer {
public:
    raytracer(){};
    std::vector<Pixel_t> render(Render_t render);
};

#endif  // RAYTRACER_H