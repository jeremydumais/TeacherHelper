#pragma once

#include <IFileSystemOperations.h>
#include <string>

class IDatabaseManagementOperations
{
public:
    virtual ~IDatabaseManagementOperations() {};
    virtual const std::string &getLastError() const = 0;
    virtual bool exist(const std::string &databaseName) const = 0;
    virtual bool create(const std::string &databaseName) = 0;
};