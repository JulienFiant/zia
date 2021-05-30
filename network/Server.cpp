#include "Server.hpp"

Server::Server()
{
    this->state_serv = 0;
}

Server::Server(const int &port)
{
    this->port = port;
    this->state_serv = 0;
    this->startServer();
}

Server::~Server()
{
    for (auto it : this->list_tmp)
    {
        if (it->joinable())
            it->join();
        delete (it);
    }
    for (auto it : this->module_list)
        delete (it);
    std::cout << "All Client disconnected" << std::endl;
    std::cout << "Server closed" << std::endl;
}

void Server::setPort(const int &port)
{
    if (this->port == port)
        return;
    this->port = port;
    this->startServer();
}

void Server::startServer()
{
    if (this->state_serv == 1)
    {
        std::cout << "hello" << std::endl;
        listen(this->sock, 0);
        close(this->sock);
        this->state_serv = 0;
    }
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->sock == -1)
    {
        std::cout << "Error socket return -1" << std::endl;
        return;
    }
    int enable = 1;
    if (setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cout << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        return;
    }
    this->serv.sin_addr.s_addr = INADDR_ANY;
    this->serv.sin_family = AF_INET;
    this->serv.sin_port = htons(this->port);
    if (bind(this->sock, (struct sockaddr *)(&this->serv), sizeof(struct sockaddr)) == -1)
    {
        std::cout << "Error bind" << std::endl;
        return;
    }
    std::cout << "Server Open" << std::endl;
    this->state_serv = 1;
    listen(this->sock, 5);
    std::cout << inet_ntoa(this->serv.sin_addr) << std::endl;
}

void Server::acceptCli()
{
    if (this->state_serv != 1)
    {
        std::cout << "Error on config, so change it" << std::endl;
        return;
    }
    int tmp_cli = accept(this->sock, 0, 0);

    if (tmp_cli != -1)
    {
        /* Ouai ouai, la pipeline
        this->pipeline->onReceiveHooks.push_back(std::pair(
            [this, tmp_cli](const bref::Environment &environment) {
                return (
                    [this, tmp_cli](bref::SocketType socket, bref::Buffer &buffer) {
                        this->list_tmp.push_back(new std::thread(&Server::start_thread, this, tmp_cli));
                        return true;
                    });
            },
            0.0));
        (this->pipeline->onReceiveHooks.back.first())():
*/
        this->list_tmp.push_back(new std::thread(&Server::start_thread, this, tmp_cli));
    }
}

void Server::setModule(const std::list<Module *> &module_list)
{
    this->mutex.lock();
    this->module_list = module_list;
    this->mutex.unlock();
}

void Server::start_thread(const int &tmp_cli)
{
    std::cout << "New client" << std::endl;
    Client *tmp = new Client(tmp_cli);
    tmp->Receive();
    if (this->getPhp() != nullptr)
        tmp->setPhp(new Module(this->getPhp()->getName(), this->getPhp()->getPath()));
    tmp->ParseRequest();
    delete (tmp);
}

Module *Server::getPhp()
{
    this->mutex.lock();
    for (auto it : this->module_list)
    {
        if (it->getName() == "php" || it->getName() == "php-cgi")
        {
            this->mutex.unlock();
            return (it);
        }
    }
    this->mutex.unlock();
    return (nullptr);
}