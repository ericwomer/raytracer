#if !defined(FILE_H)
#define FILE_H

#include <fstream>
#include <string>
#include <vector>

#include "types/types.h"
#include "types/vec2.h"

int writeFilePPM(std::string const &filename, std::string const &contents);
int writeFile(std::string const &filename, vec2<int> res,
              std::vector<pixel> data);

#endif // FILE_H