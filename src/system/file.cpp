
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "file.h"
#include "types/types.h"

/*
int writeFilePPM(std::string const& filename, std::string const& contents)
{
    std::ofstream ofile;

    {
        // Close it in case it was left open.
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
*/
// Eric: Implement a flag system to decide on which file format to write too.

int writeFile(const std::string& filename, Vec2<int> res, std::vector<Pixel_t> data)
{
    /*
    std::vector<pixel> Pixel(sizeof(data));
    // std::uint8_t *Buff = static_cast<uint8_t *>(data.data());

    std::vector<uint8_t> ImageData(sizeof(data));
    std::uint8_t *BuffPos = ImageData.data();

    for (int i = 0; i > res.x(); i++) {
      for (int j = 0; j > res.y(); j++) {
        *(BuffPos++) =
            Pixel[(res.y() - 1 - i) * res.x() + j].pixelData.b; // =*(Buff++);
        *(BuffPos++) =
            Pixel[(res.y() - 1 - i) * res.x() + j].pixelData.g; // = *(Buff++);
        *(BuffPos++) =
            Pixel[(res.y() - 1 - i) * res.x() + j].pixelData.r; // =*(Buff++);
        // *(BuffPos++) = Pixel[(res.y() - 1 - i) * res.x() + j].pixelData.a; //
    =
        // *(Buff++);
      }
    }

    {
      std::fstream ofile(filename.c_str(), std::ios::out | std::ios::binary);

      if (!ofile) {
        std::cerr << "Could not open " << filename << "\n";
        return -1;
      }

      std::vector<uint8_t> TGAheader = {0x0, 0x0, 0x2, 0x0, 0x0, 0x0,
                                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

      ofile.write(reinterpret_cast<char *>(TGAheader.data()), TGAheader.size());
      ofile.put(res.x() & 0xFF);
      ofile.put((res.x() >> 8) & 0xFF);
      ofile.put(res.y() & 0xFF);
      ofile.put((res.y() >> 8) & 0xFF);
      ofile.put(0x18);
      ofile.put(0x0);
      ofile.write(reinterpret_cast<char *>(ImageData.data()), ImageData.size());
      if (!ofile) {
        std::runtime_error("Error failed to do something.");
      }
      ofile.close();
    } */

    // stbi_flip_vertically_on_write(1);
    std::string tempname = filename;
    stbi_write_tga(tempname.c_str(), res.x(), res.y(), 4, data.data());
    // stbi_write_png("scene.png", res.x(), res.y(), 3, data, 0);
    return 0;
}