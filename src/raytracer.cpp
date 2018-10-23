
#include "raytracer.h"

int writeFile(std::string &filename, std::string &contents) {
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

raytracer::raytracer() {
	app_description.push_back(std::string("Raytracer \n"));
	app_description.push_back(std::string("** nothing else follows ** \n"));
}

// All of the application starts here
// if any parameters are used they are handled
// with member vars.
int raytracer::main(void) {
	// Start Main Application Here.
	// writePlaylist();
	return 0;
}

// This is the main that handles parameters
int raytracer::main(std::vector<std::string> &params) {

	// std::vector<std::string> actions;
	std::vector<std::string> dump;

	// iterate through params to remove the -- from the text
	for (std::vector<std::string>::const_iterator i = params.begin();
	     i != params.end(); ++i) {

		if (*i == "--help" || *i == "-h") {
			actions.push_back("help");
		} else if (*i == "--verbose" || *i == "-v") {
			actions.push_back("verbose");
		} else if (*i == "--version" || *i == "-V") {
			actions.push_back("version");
		} else { // catch all to make sure there are no invalid parameters
			dump.push_back(*i);
		}
	}

	for (auto &c : actions) { // handle all the prameters
		if (c == "help") {
			help();
			return 0; // exit the app

		} else if (c == "verbose") {

			for (auto &d : actions) {
				std::cout << "--" << d << " ";
			}
			std::cout << std::endl;
			return 0;
		} else if (c == "version") {
			std::cout << app_name << " " << 00 << "." << 00 << "." << 01
			          << std::endl; // create a version raytracer class??
			return 0;
		} else {
			dump.push_back(c);
		}
	}

	return main();
}

// This main converts c style parameters to c++ strings
// then passes it to main that handles the actual parametrs.
int raytracer::main(int argv, char *argc[]) {
	// Start here if there are params
	std::vector<std::string> params;

	setAppName(argc[0]);

	for (int i = 1; i != argv; ++i) {
		params.push_back(argc[i]);
	}

	return main(params);
}

void raytracer::help(void) {
	std::cout << "Usage: " << app_name << " [options] files...\n\n";
	std::cout << "Options: \n";
	std::cout
	    << " -h, --help \t\t Print this help message and exit the program.\n";
	std::cout << " -v, --verbose \t\t Print out all the valid command line "
	             "parameters\n";
	std::cout << " \t\t\t passed to the program.\n";
	std::cout << " -V, --version \t\t Print the version and exit.\n";
}
