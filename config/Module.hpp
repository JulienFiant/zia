#ifndef MODULE_HPP
#define MODULE_HPP

#include "../execution_cmd/Execute.hpp"
#include <string>

class Module
{
public:
    Module(const std::string &name, const std::string &path);
    ~Module();
    std::string getName() const;
    std::string getPath() const;
    std::string executeCmd(const std::string &cmd);

protected:
    Execute exec;
    std::string name;
    std::string path;
};

#endif