#pragma once

#include "databaseConnection.h"
#include "operationBase.h"
#include <string>
#include <vector>

class SQLiteUpdateOperation : public OperationBase
{
public:
    SQLiteUpdateOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool Execute() override;
};