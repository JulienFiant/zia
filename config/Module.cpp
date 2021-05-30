#include "Module.hpp"

Module::Module(const std::string &name, const std::string &path)
{
    this->name = name;
    this->path = path;
}

Module::~Module()
{
}

std::string Module::getName() const
{
    return (this->name);
}

std::string Module::getPath() const
{
    return (this->path);
}

std::string Module::executeCmd(const std::string &cmd)
{
    std::unordered_map<std::string, std::string> env;

    if (this->name == "php" || this->name == "php-cgi")
    {
        env["REDIRECT_STATUS"] = "200";
        env["GATEWAY_INTERFACE"] = "CGI/1.1";
        env["SCRIPT_FILENAME"] = cmd;
    }
    return (this->exec.run(this->path + "/" + this->name + " " + cmd, env));
}