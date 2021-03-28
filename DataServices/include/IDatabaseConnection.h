#pragma once

#include <string>

class IDatabaseConnection
{
public:
    virtual ~IDatabaseConnection() {};
    virtual const std::string &getDbName() const = 0;
    virtual void *getConnectionPtr() const = 0;
    virtual void open() = 0;
    virtual void openForCreation() = 0;
    virtual bool isOpened() const = 0;
    virtual void close() = 0;
};