#include "ConfigManager.hpp"

ConfigManager::ConfigManager()
{
    this->path = "config.conf";
    this->paths_ = std::filesystem::last_write_time(this->path);
    this->Init();
    this->run = true;
    this->tmp = std::thread(&ConfigManager::start, this);
}

ConfigManager::ConfigManager(const std::string &path)
{
    this->path = path;
    this->paths_ = std::filesystem::last_write_time(this->path);
    this->Init();
    this->run = true;
    this->tmp = std::thread(&ConfigManager::start, this);
}

ConfigManager::~ConfigManager()
{
    if (tmp.joinable())
    {
        this->run = false;
        tmp.join();
    }
}

std::vector<std::string> ConfigManager::split(const std::string &s, const std::string &delimiter)
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

void ConfigManager::Init()
{
    std::string line = "";
    std::ifstream outfile;
    std::vector<std::string> tmp;
    outfile.open(this->path);
    if (outfile.is_open())
    {
        this->list_mutex.lock();
        while (std::getline(outfile, line))
        {
            tmp = this->split(line, "=");
            if (tmp.size() == 2 and tmp[1].empty() == false)
            {
                this->list.push_back(std::make_pair(tmp[0], tmp[1]));
            }
        }
        this->checkRightConfig();
        this->list_mutex.unlock();
        outfile.close();
    }
    else
    {
        std::cerr << "Error, file so load default config" << std::endl;
        this->list_mutex.lock();
        this->checkRightConfig();
        this->list_mutex.unlock();
    }
}

void ConfigManager::Reload()
{
    this->list_mutex.lock();
    this->list.clear();
    this->list_mutex.unlock();
    this->Init();
    this->Print();
}

void ConfigManager::Print()
{
    std::cout << "ConfigManager" << std::endl;
    this->list_mutex.lock();
    for (auto it : this->list)
        std::cout << it.first << " " << it.second << std::endl;
    this->list_mutex.unlock();
    std::cout << std::endl;
}

std::list<std::pair<std::string, std::string>> ConfigManager::GetList()
{
    this->list_mutex.lock();
    auto tmp = this->list;
    this->list_mutex.unlock();
    return (tmp);
}

void ConfigManager::start()
{
    while (this->run)
    {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        auto current_file_last_write_time = std::filesystem::last_write_time(this->path);
        if (this->paths_ != current_file_last_write_time)
        {
            this->paths_ = current_file_last_write_time;
            this->Reload();
        }
    }
}

std::string ConfigManager::getIn(const std::string &search)
{
    for (auto it : this->list)
    {
        if (it.first == search)
        {
            return (it.second);
        }
    }
    return ("Error");
}

std::filesystem::file_time_type ConfigManager::getDate()
{
    this->list_mutex.lock();
    std::filesystem::file_time_type tmp = this->paths_;
    this->list_mutex.unlock();
    return (tmp);
}
void ConfigManager::checkRightConfig()
{
    if (std::stoi(this->getIn("port")) < 0)
    {
        this->list.remove_if([](std::pair<std::string, std::string> it) { return it.first == std::string("port"); });
        this->list.push_back(std::make_pair("port", PORT));
    }
    if (this->getIn("port") == "Error")
        this->list.push_back(std::make_pair("port", PORT));
    if (this->getIn("module") == "Error")
        this->list.push_back(std::make_pair("module", MODULE));
    if (this->getIn("module_path") == "Error")
        this->list.push_back(std::make_pair("module_path", MODULE_PATH));
}