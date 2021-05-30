#include "../bref/HttpRequest.h"

bref::HttpRequest::HttpRequest()
{
}

bref::HttpRequest::~HttpRequest()
{
}

bref::request_methods::Type bref::HttpRequest::getMethod() const
{
    return (this->method_);
}

const std::string &bref::HttpRequest::getUri() const
{
    return (this->uri_);
}

const bref::Version &bref::HttpRequest::getVersion() const
{
    return (this->version_);
}

void bref::HttpRequest::setMethod(bref::request_methods::Type t)
{
    this->method_ = t;
}

void bref::HttpRequest::setUri(const std::string &t)
{
    this->uri_ = t;
}

void bref::HttpRequest::setVersion(const bref::Version &t)
{
    this->version_ = t;
}