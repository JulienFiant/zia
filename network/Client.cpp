#include "Client.hpp"
#include "../bref/HttpConstants.h"

Client::Client(const int &sock)
{
    this->sock = sock;
    this->php = nullptr;
    //this->winsock.CreateSock();
}

Client::~Client()
{
    if (this->php != nullptr)
        delete (this->php);
    delete (this->httpreq);
    delete (this->httpres);
    close(this->sock);
    //this->winsock.DeleteSock(this->sock);
}

void Client::SendHttp(const std::string &msg)
{
    this->httpres = new bref::HttpResponse();
    this->httpres->setStatus(bref::status_codes::OK);
    time_t ltime;
    time(&ltime);
    tm *gmt = gmtime(&ltime);
    char *asctime_remove_nl = asctime(gmt);
    asctime_remove_nl[24] = 0;
    std::string ans = "HTTP/1.1 " + std::to_string(this->httpres->getStatus()) + " OK\n";
    ans += "Date: " + std::string(asctime_remove_nl) + " GMT\n";
    ans += "Connection: close\n";
    ans += "Server: CPP\n";
    ans += "Accept-Ranges: bytes\n";
    ans += "Content-Type: text/html\n";
    ans += "Content-Length: " + std::to_string(msg.length()) + "\n";
    ans += "Last-Modified: Sat Feb 15 9:18:29 2020 GMT\n";
    ans += "\r\n";
    ans += msg;
    this->httpres->setReason(ans);
    std::string send_msg = this->httpres->getReason();
    send(this->sock, send_msg.c_str(), send_msg.length(), 0);
    std::cout << "Send" << std::endl;
}

std::string Client::Receive()
{
    char buffer[200] = {0};
    recv(this->sock, buffer, 200, 0);
    std::string tmp = buffer;
    this->req = tmp;
    this->httpreq = new bref::HttpRequest();
    if (this->req.find("GET") != std::string::npos)
        this->httpreq->setMethod(bref::request_methods::Type::Get);
    else
        this->httpreq->setMethod(bref::request_methods::Type::UndefinedRequestMethod);
    std::vector<std::string> tmp2 = this->split(this->req, " ");
    this->httpreq->setUri(tmp2[1]);
    return (tmp);
}

void Client::ParseRequest()
{
    if (this->httpreq->getMethod() == bref::request_methods::Type::Get)
    {
        std::cout << "This is Get Request" << std::endl;
        this->GetMethod();
    }
    else
    {
        std::cout << "This is not a Get Request" << std::endl;
        this->SendHttp("<html><body><h1> Not a Get Request </h1></body></html>\n");
    }
}

void Client::GetMethod()
{
    std::string tmp_path = "./www" + this->httpreq->getUri();
    std::string html;
    std::string line;
    int type = bref::request_methods::Type::Get;

    if (!std::filesystem::exists(tmp_path) || this->httpreq->getUri() == "/")
    {
        std::cout << "Error Path: " << tmp_path << std::endl;
        this->SendHttp("<html><body><h1> Page Not found</h1></body></html>\n");
        return;
    }
    else
        std::cout << "File exist" << std::endl;
    /* TODO send code error html */
    if (tmp_path.find(".ph") != std::string::npos)
    {
        if (this->php != nullptr)
        {
            html = this->php->executeCmd(tmp_path);
        }
        else
            html = "<html><body><h1> No module Php </h1></body></html>\n";
        this->SendHttp(html);
        return;
    }
    std::ifstream outfile;
    outfile.open(tmp_path);
    if (outfile.is_open())
    {
        while (std::getline(outfile, line))
        {
            html += line;
        }
        outfile.close();
        this->SendHttp(html);
    }
}

void Client::PostMethod()
{
}

std::vector<std::string> Client::split(const std::string &s, const std::string &delimiter)
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

void Client::setPhp(Module *php)
{
    this->php = php;
}