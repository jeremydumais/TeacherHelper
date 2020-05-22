#pragma once

#include "databaseConnection.h"
#include <vector>

class OperationBase
{
public: 
    OperationBase(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : dbConnection(&connection),
          query(query),
          args(args),
          lastError("") 
    {
    }
    virtual ~OperationBase() {}
    virtual bool execute() = 0;
    const std::string &getLastError() const
    {
        return lastError;
    }
protected:
    const DatabaseConnection *dbConnection;
    std::string query;
    std::vector<std::string> args;
    std::string lastError;
};