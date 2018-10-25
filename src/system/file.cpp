
#include <iostream>

#include "file.hpp"

int writeFile(std::string const &filename, std::string const &contents) {
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