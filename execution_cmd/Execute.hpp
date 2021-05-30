#ifndef EXECUTE_HPP
#define EXECUTE_HPP

#include <string>
#include <unistd.h>
#include <wait.h>
#include <iostream>
#include <algorithm>
#include <memory.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <fstream>

class Execute
{
public:
    Execute();
    ~Execute();
    std::string run(const std::string &cmd, const std::unordered_map<std::string, std::string> &test_env);
    void openfd();
    void startCmd();
    void exeCmd();
    void getResult();
    void closeHost();
    void waitCmd() const;
    char **generateEnv(const std::unordered_map<std::string, std::string> &envMap) const;
    std::vector<std::string> split(const std::string &s, const std::string &delimiter);
    std::string readFile(const std::string &path);

protected:
    std::string cmd;
    std::string res;
    int _inputFds[2]{};
    int _ouptutFds[2]{};
    pid_t _pid;
    std::unordered_map<std::string, std::string> env;
};

#endif