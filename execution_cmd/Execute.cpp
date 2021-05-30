#include "Execute.hpp"

/* TODO if error react */

Execute::Execute()
{
}

Execute::~Execute()
{
}

std::string Execute::run(const std::string &cmd, const std::unordered_map<std::string, std::string> &test_env)
{
    this->env = test_env;
    this->cmd = cmd;
    this->openfd();
    this->startCmd();
    std::string data = this->readFile(this->split(this->cmd, " ")[1]);
    write(_inputFds[1], data.c_str(), data.size());
    close(_inputFds[1]);
    this->getResult();
    this->waitCmd();
    this->closeHost();
    std::string tmp;
    int i = 0;
    while (this->res[i] && this->res[i] != '<')
        ++i;
    if (this->res.size() == i)
        return ("<html><body><h1> Structure not good</h1></body></html>");
    while (this->res[i])
    {
        tmp += this->res[i];
        ++i;
    }
    this->res = tmp;
    return this->res;
}

void Execute::openfd()
{
    if (pipe(this->_inputFds) != 0 || pipe(this->_ouptutFds) != 0)
        std::cout << "Error Pipe" << std::endl;
    // todo throw exception
}

void Execute::startCmd()
{
    this->_pid = fork();
    switch (_pid)
    {
    case -1:
        std::cout << "Error Fork" << std::endl;
        // todo throw exception
    case 0:
        this->exeCmd();
        break;
    default:
        this->closeHost();
    }
}

void Execute::exeCmd()
{
    std::vector<std::string> tmp = this->split(this->cmd, " ");
    char *params[2] = {const_cast<char *>(tmp[1].c_str()), nullptr};
    close(this->_inputFds[1]);
    close(this->_ouptutFds[0]);
    dup2(this->_ouptutFds[1], 1);
    dup2(this->_inputFds[0], 0);
    execve(tmp[0].c_str(), params, this->generateEnv(this->env));
    std::exit(0);
}

void Execute::getResult()
{
    char buff[1024];
    long int size;

    size = read(this->_ouptutFds[0], buff, 1023);
    while (size > 0)
    {
        if (size > 0)
        {
            buff[size] = '\0';
            this->res += buff;
        }
        size = read(this->_ouptutFds[0], buff, 1023);
    }
}

void Execute::waitCmd() const
{
    waitpid(this->_pid, nullptr, 0);
}

char **Execute::generateEnv(const std::unordered_map<std::string, std::string> &envMap) const
{
    auto **env = new char *[envMap.size() + 1];
    unsigned int registered = 0;
    std::for_each(envMap.begin(), envMap.end(), [&](const std::pair<std::string, std::string> &pair) {
        env[registered++] = strdup(std::string(pair.first + "=" + pair.second).c_str());
    });
    env[envMap.size()] = nullptr;
    return (env);
}

std::vector<std::string> Execute::split(const std::string &s, const std::string &delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::string Execute::readFile(const std::string &path)
{
    std::ifstream outfile;
    outfile.open(path);
    std::string php;
    std::string line;
    if (outfile.is_open())
    {
        while (std::getline(outfile, line))
        {
            php += line;
        }
        outfile.close();
    }
    return (php);
}

void Execute::closeHost()
{
    close(this->_inputFds[0]);
    close(this->_ouptutFds[1]);
}