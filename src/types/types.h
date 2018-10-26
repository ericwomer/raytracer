#if !defined(TYPES_H)
#define TYPES_H

#include <cstdint>
#include <string>

typedef uint8_t byte;
typedef uint32_t byte32;

// Move to raytracer class
typedef struct pixelrgb_s {
	byte r;
	byte g;
	byte b;
} pixelrgb_t;

typedef struct pixelrgba_s {
	byte r;
	byte g;
	byte b;
	byte a;
} pixelrgba_t;

typedef struct hdr_pixelrgb_s {
	byte32 r;
	byte32 g;
	byte32 b;
} hdr_pixelrgb_t;

typedef struct hdr_pixelrgba_s {
	byte32 r;
	byte32 g;
	byte32 b;
	byte32 a;
} hdr_pixelrgba_t;
// End move to raytracer class

// Move to file i/o class
enum writefile_ext : int8_t { WRITE_PPM, WRITE_PNG, WRITE_JPEG, WRITE_TGA };

#endif // TYPES_H