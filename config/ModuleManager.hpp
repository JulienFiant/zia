#ifndef MODULEMANAGER_HPP
#define MODULEMANAGER_HPP

#include "Module.hpp"
#include <list>
#include <vector>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <thread>

class ModuleManager
{
public:
    ModuleManager(const std::string &list_module, const std::string &module_path, std::filesystem::file_time_type date);
    ~ModuleManager();
    int InitListModule(const std::string &list_module, const std::string &module_path);
    int addModule(const std::string &name, const std::string &path);
    int removeModule(const std::string &name);
    std::list<Module *> getModuleList();
    std::vector<std::string> split(const std::string &s, const std::string &delimiter);
    void Print();
    int Count();
    int CheckIfModif(const std::filesystem::file_time_type &check, const std::string &list_module, std::string module_path);
    int Reload(const std::string &list_module, const std::string &module_path);

protected:
    std::list<Module *> module_list;
    std::mutex list_mutex;
    std::filesystem::file_time_type date;
};

#endif