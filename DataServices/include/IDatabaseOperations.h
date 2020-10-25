#pragma once

#include <sqlite3.h>
#include <string>

class IDatabaseOperations
{
public:
    virtual ~IDatabaseOperations() {};
    virtual int open(const std::string &fileName) = 0;
    virtual int openForCreation(const std::string &fileName) = 0;
    virtual int close() = 0;
    virtual void *getConnectionPtr() const = 0;
};