#include "ModuleManager.hpp"

ModuleManager::ModuleManager(const std::string &list_module, const std::string &module_path, std::filesystem::file_time_type date)
{
    this->InitListModule(list_module, module_path);
    this->date = date;
}

ModuleManager::~ModuleManager()
{
    for (auto it : this->module_list)
        delete (it);
    this->module_list.clear();
}

int ModuleManager::InitListModule(const std::string &list_module, const std::string &module_path)
{
    int i = 0;
    this->list_mutex.lock();
    std::vector<std::string> tmp_list = this->split(list_module, ",");
    std::vector<std::string> tmp_path = this->split(module_path, ",");
    for (int i = 0; i < tmp_list.size(); i += 1)
    {
        for (int j = 0; j < tmp_path.size(); j += 1)
        {
            if (std::filesystem::exists(tmp_path[j] + "/" + tmp_list[i]))
            {
                this->module_list.push_back(new Module(tmp_list[i], tmp_path[j]));
                break;
            }
        }
    }
    i = this->module_list.size();
    this->list_mutex.unlock();
    return (i);
}

int ModuleManager::addModule(const std::string &name, const std::string &path)
{
    int i = 0;
    this->list_mutex.lock();
    if (std::filesystem::exists(path + "/" + name))
        this->module_list.push_back(new Module(name, path));
    else
        std::cout << "Error" << std::endl;
    i = this->module_list.size();
    this->list_mutex.unlock();
    return (i);
}

int ModuleManager::removeModule(const std::string &name)
{
    int i = 0;
    this->list_mutex.lock();
    auto itt = this->module_list.begin();
    for (auto it : this->module_list)
    {
        if (it->getName() == name)
        {
            this->module_list.erase(itt);
            delete (it);
            i = this->module_list.size();
            this->list_mutex.unlock();
            return (i);
        }
        ++itt;
    }
    i = this->module_list.size();
    this->list_mutex.unlock();
    return (i);
}

std::list<Module *> ModuleManager::getModuleList()
{
    this->list_mutex.lock();
    std::list<Module *> tmp = this->module_list;
    this->list_mutex.unlock();
    return (tmp);
}

std::vector<std::string> ModuleManager::split(const std::string &s, const std::string &delimiter)
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

void ModuleManager::Print()
{
    std::cout << "ModuleManager" << std::endl;
    this->list_mutex.lock();
    for (auto it : this->module_list)
        std::cout << it->getName() << std::endl;
    this->list_mutex.unlock();
    std::cout << std::endl;
}

int ModuleManager::Count()
{
    this->list_mutex.lock();
    int tmp = this->module_list.size();
    this->list_mutex.unlock();
    std::cout << tmp << std::endl;
    return (tmp);
}

int ModuleManager::CheckIfModif(const std::filesystem::file_time_type &check, const std::string &list_module, std::string module_path)
{
    if (this->date != check)
    {
        this->date = check;
        this->Reload(list_module, module_path);
        return (1);
    }
    return (0);
}

int ModuleManager::Reload(const std::string &list_module, const std::string &module_path)
{
    this->list_mutex.lock();
    for (auto it : this->module_list)
        delete (it);
    this->module_list.clear();
    int i = 0;
    std::vector<std::string> tmp_list = this->split(list_module, ",");
    std::vector<std::string> tmp_path = this->split(module_path, ",");
    for (int i = 0; i < tmp_list.size(); i += 1)
    {
        for (int j = 0; j < tmp_path.size(); j += 1)
        {
            if (std::filesystem::exists(tmp_path[j] + "/" + tmp_list[i]))
            {
                this->module_list.push_back(new Module(tmp_list[i], tmp_path[j]));
                break;
            }
        }
    }
    i = this->module_list.size();
    this->list_mutex.unlock();
    this->Print();
    return (i);
}