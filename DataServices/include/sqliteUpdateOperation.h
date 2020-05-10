#pragma once

#include "operationBase.h"
#include <string>
#include <vector>

class SQLiteUpdateOperation : public OperationBase
{
public:
    SQLiteUpdateOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
};