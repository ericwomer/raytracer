#if !defined(__TYPES_HPP__)
#define __TYPES_HPP__

#include <cstdint>

typedef unsigned char byte;
typedef uint32_t hdr_byte;

// Move to raytracer class
typedef struct pixelrgb {
	byte r;
	byte g;
	byte b;
};

typedef struct pixelrgba {
	byte r;
	byte g;
	byte b;
	byte a;
};

typedef struct hdr_pixelrgb {
	hdr_byte r;
	hdr_byte g;
	hdr_byte b;
};

typedef struct hdr_pixelrgba {
	hdr_byte r;
	hdr_byte g;
	hdr_byte b;
	hdr_byte a;
};
// End move to raytracer class

// Move to file i/o class
enum class e_writefile_ext : int8_t {
	WRITE_PPM,
	WRITE_PNG,
	WRITE_JPEG,
	WRITE_TGA
};

#endif // __TYPES_HPP__