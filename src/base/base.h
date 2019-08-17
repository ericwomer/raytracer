#ifndef BASE_H
#define BASE_H

#include <string>
#include <vector>

class Base {
public:  // data types
    // Public Interface
    Base(){};
    virtual ~Base(){};
    virtual int               main() = 0;
    virtual int               main(std::vector<std::string>& params) = 0;
    virtual int               size() = 0;
    virtual const std::string name() = 0;
    virtual void              help() = 0;
    virtual void              name(std::string name) = 0;
    virtual void              version() = 0;

protected:
    // Protected data members
    std::string              app_name;
    std::vector<std::string> app_description;

    std::string version_number;
};

#endif  // BASE_H
