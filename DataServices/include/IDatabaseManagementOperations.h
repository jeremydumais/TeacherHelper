#pragma once

#include <string>

class IDatabaseManagementOperations
{
public:
    virtual ~IDatabaseManagementOperations() {};
    virtual bool backup(const std::string &databaseName, const std::string &backupDatabaseName) = 0;
};