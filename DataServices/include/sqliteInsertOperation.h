#pragma once

#include "operationBase.h"
#include <string>

class SQLiteInsertOperation : public OperationBase
{
public:
    SQLiteInsertOperation(const DatabaseConnection &connection, 
                          const std::string &query);
    bool Execute() override;
};