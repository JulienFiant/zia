#ifndef CONFIGMANAGER_HPP
#define CONFIGMANAGER_HPP

#include "../thread/IThread.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <filesystem>
#include <chrono>
#include <thread>
#include <functional>
#include <atomic>
#include <mutex>

#define PORT "9000"
#define MODULE "php-cgi"
#define MODULE_PATH "/usr/bin"

class ConfigManager : public IThread
{
public:
    ConfigManager();
    ConfigManager(const std::string &path);
    ~ConfigManager();
    std::vector<std::string> split(const std::string &s, const std::string &delimiter);
    void Init();
    void Reload();
    void Print();
    std::list<std::pair<std::string, std::string>> GetList();
    void start();
    std::string getIn(const std::string &search);
    std::filesystem::file_time_type getDate();
    void checkRightConfig();
    bool isAlive() { return true; };

protected:
    std::string path;
    std::list<std::pair<std::string, std::string>> list;
    std::filesystem::file_time_type paths_;
    std::thread tmp;
    std::atomic_bool run;
    std::mutex list_mutex;
};

#endif