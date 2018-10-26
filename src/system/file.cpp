
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "file.h"

int writeFilePPM(std::string const &filename, std::string const &contents) {
	std::ofstream ofile;

	{

		// Close it incase it was left open.
		if (ofile.is_open()) {
			ofile.close();
		}
		ofile.open(filename);

		if (!ofile) {
			std::cerr << "Could not open " << filename << "\n";
			return -1;
		}

		ofile << contents;
		ofile.close();
	}

	return 0;
}

// Eric: Implement a flag system to decide on which file format to write too.
int writeFile(const std::string &filename, vec2<int> res, void *data) {
	stbi_flip_vertically_on_write(1);
	std::string tempname = filename;
	stbi_write_tga(tempname.c_str(), res.x(), res.y(), 3, data);
	return 0;
}