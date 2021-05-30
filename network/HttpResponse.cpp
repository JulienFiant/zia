#include "../bref/HttpResponse.h"

bref::HttpResponse::HttpResponse()
{
}

bref::HttpResponse::~HttpResponse()
{
}

const bref::Version &bref::HttpResponse::getVersion() const
{
    return (this->version_);
}

bref::status_codes::Type bref::HttpResponse::getStatus() const
{
    return (this->statusCode_);
}

const std::string &bref::HttpResponse::getReason() const
{
    return (this->reason_);
}

bref::Buffer bref::HttpResponse::getRawData() const
{
    bref::Buffer t;
    return (t);
}

void bref::HttpResponse::setVersion(const Version &version)
{
    this->version_ = version;
}

void bref::HttpResponse::setStatus(status_codes::Type type)
{
    this->statusCode_ = type;
}

void bref::HttpResponse::setReason(const std::string &reason)
{
    this->reason_ = reason;
}