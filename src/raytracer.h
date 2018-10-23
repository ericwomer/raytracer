#if !defined(__RAYTRACER_H__)
#define __RAYTRACER_H__

#include <string>
#include <vector>

#include <fstream>
#include <iostream>

#include <cstdint>

#include "base/base.h"

class raytracer : public base {
public: // data types
	typedef struct versionNumberData_t {
		uint8_t major;
		uint8_t minor;
		uint8_t patch;
	} versionNumberData, *pversionNumberData;

public:
	raytracer();
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
};

#endif //__RAYTRACER_H__