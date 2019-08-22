#if !defined(TYPES_H)
#define TYPES_H

#include <cstdint>
#include <string>

#include "vec2.h"
#include "vec3.h"

#include "rendering/ray.h"
#include "objects/hitable.h"
#include "materials/material.h"
#include "camera/camera.h"

// typedef uint8_t byte;
// typedef uint32_t byte32;

// Move to file i/o class
enum writefile_ext : int8_t
{
    WRITE_PPM,
    WRITE_PNG,
    WRITE_JPEG,
    WRITE_TGA
};

// internal pixel format
typedef union Pixel_u {
    std::uint32_t Color;
    struct {
        std::uint8_t r, g, b, a;
    } pixelData;
} Pixel_t;

// Eric: Placeholder for now, don't pass this down to other functions yet
typedef struct OutFile_s {  // for a lack of a better name
    std::string  name;
    std::string  ext;
} OutFile_t;

typedef struct InFile_s {
    std::string name;
    std::string ext;
}InFile_t;

typedef struct Render_s {
    camera* cam;
    Vec2<int>    res;
    unsigned int samples;
    hitable* world;
    std::size_t grains;
    bool        threaded;
    std::vector<Pixel_t> pixels;
} Render_t;

// Move this eventually to types
inline Vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<double>::max(), rec)) {
        ray  scattered;
        Vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return Vec3(0, 0, 0);
        }
    } else {
        Vec3   unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
}

#endif  // TYPES_H