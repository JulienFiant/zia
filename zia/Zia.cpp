#include "Zia.hpp"

Zia::Zia()
{
    this->m = new ConfigManager();
    this->mm = new ModuleManager(this->m->getIn("module"), this->m->getIn("module_path"), this->m->getDate());
    this->serv = new Server(std::stoi(this->m->getIn("port")));
    this->run = true;
    this->tmp = std::thread(&Zia::start_thread, this);
}

Zia::~Zia()
{
    if (tmp.joinable())
    {
        this->run = false;
        tmp.join();
    }
    delete (this->m);
    delete (this->mm);
    delete (this->serv);
}

int Zia::Core()
{
    this->m->Print();
    this->mm->Print();
    this->serv->setModule(this->mm->getModuleList());

    while (1)
    {
        this->serv->acceptCli();
    }
    return (0);
}

void Zia::start_thread()
{
    int i = 0;
    while (this->run)
    {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        i = this->mm->CheckIfModif(this->m->getDate(), this->m->getIn("module"), this->m->getIn("module_path"));
        if (i == 1)
        {
            this->serv->setPort(std::stoi(this->m->getIn("port")));
            this->serv->setModule(this->mm->getModuleList());
        }
    }
}