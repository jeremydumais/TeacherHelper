#pragma once

#include "IDatabaseConnection.h"
#include <vector>

class OperationBase
{
public: 
    OperationBase(const IDatabaseConnection &connection, 
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
    const IDatabaseConnection *dbConnection;
    std::string query;
    std::vector<std::string> args;
    std::string lastError;
};