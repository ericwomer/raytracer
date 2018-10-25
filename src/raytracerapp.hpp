#if !defined(__RAYTRACERAPP_H__)
#define __RAYTRACERAPP_H__

#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <cstdint>

#include "base/base.hpp"
#include "camera/camera.hpp"
#include "materials/material.hpp"
#include "objects/sphere.hpp"
#include "rendering/hitable_list.hpp"
#include "rendering/raytracer.hpp"
#include "system/file.hpp"
#include "system/system.hpp"

class raytracerapp : public base {
public: // data types
	typedef struct versionNumberData_t {
		uint8_t major;
		uint8_t minor;
		uint8_t patch;
	} versionNumberData, *pversionNumberData;

public:
	raytracerapp();
	virtual int main(void);
	virtual int main(int argv, char *argc[]);
	virtual int main(std::vector<std::string> &params);
	virtual int size(void) { return sizeof(this); };
	virtual std::string name(void) { return app_name; };
	virtual void help(void);
	virtual void setAppName(std::string name) { app_name = name; };

private:
	std::vector<std::string> actions;

	std::string app_name;
	std::vector<std::string> app_description;

	versionNumberData versionNumber = {0, 0, 0};

	raytracer tracer;
};

/**
 * @brief Eventually turn this in to a valid progress indicator.
 *
 */
/*
class pBar {
public:
  void update(double newProgress) {
    currentProgress += newProgress;
    amountOfFiller =
        (int)((currentProgress / neededProgress) * (double)pBarLength);
  }
  void print() {
    currUpdateVal %= pBarUpdater.length();
    std::cout << "\r"                          // Bring cursor to start of line
              << firstPartOfpBar;              // Print out first part of pBar
    for (int a = 0; a < amountOfFiller; a++) { // Print out current progress
      std::cout << pBarFiller;
    }
    std::cout << pBarUpdater[currUpdateVal];
    for (int b = 0; b < pBarLength - amountOfFiller; b++) { // Print out spaces
      std::cout << " ";
    }
    std::cout << lastPartOfpBar // Print out last part of progress bar
              << " (" << (int)(100 * (currentProgress / neededProgress))
              << "%)" // This just prints out the percent
              << std::flush;
    currUpdateVal += 1;
  }
  std::string firstPartOfpBar =
                  "[", // Change these at will (that is why I made them public)
      lastPartOfpBar = "]", pBarFiller = "|", pBarUpdater = "/-\\|";

private:
  int amountOfFiller,
      pBarLength = 50,        // I would recommend NOT changing this
      currUpdateVal = 0;      // Do not change
  double currentProgress = 0, // Do not change
      neededProgress = 100;   // I would recommend NOT changing this
}; */

#endif //__RAYTRACERAPP_H__