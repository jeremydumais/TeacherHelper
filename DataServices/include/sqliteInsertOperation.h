#pragma once

#include "operationBase.h"
#include <string>
#include <vector>

class SQLiteInsertOperation : public OperationBase
{
public:
    SQLiteInsertOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
};