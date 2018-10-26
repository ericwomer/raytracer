#if !defined(__FILE_HPP__)
#define __FILE_HPP__

#include <fstream>
#include <string>
#include <vector>

#include "types/vec2.hpp"

int writeFilePPM(std::string const &filename, std::string const &contents);
int writeFile(std::string const &filename, vec2<int> res, void *data);

#endif // __FILE_HPP__