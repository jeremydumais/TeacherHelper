#pragma once

#include "operationBase.h"
#include <string>
#include <vector>

class SQLiteDDLOperation : public OperationBase
{
public:
    SQLiteDDLOperation(const DatabaseConnection &connection, 
                          const std::string &query);
    bool execute() override;
};