#ifndef BASE_H
#define BASE_H

#include <string>
#include <vector>

class base {
public: // data types
	typedef struct versionNumberData_t {
		uint8_t major;
		uint8_t minor;
		uint8_t patch;
	} versionNumberData, *pversionNumberData;

public:
	base(){};
	virtual ~base(){};
	virtual int main(void) = 0;
	virtual int main(int argv, char *argc[]) = 0;
	virtual int main(std::vector<std::string> &params) = 0;
	virtual int size(void) { return sizeof(this); };
	virtual std::string name(void) = 0;
	virtual void help(void) = 0;
	virtual void setAppName(std::string name) = 0;
	virtual versionNumberData version(void) { return versionNumber; };

private:
	std::string app_name;
	std::vector<std::string> app_description;

	versionNumberData versionNumber;
};

#endif // PRIMER_H
