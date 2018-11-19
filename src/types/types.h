#if !defined(TYPES_H)
#define TYPES_H

#include <cstdint>
#include <string>

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

#endif  // TYPES_H