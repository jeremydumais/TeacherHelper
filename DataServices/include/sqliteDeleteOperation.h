#pragma once

#include "operationBase.h"
#include "queryResult.h"
#include <string>
#include <vector>

class SQLiteDeleteOperation : public OperationBase
{
public:
    SQLiteDeleteOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
    QueryResult getExtendedResultInfo() const;
private:
    QueryResult extendedResultInfo;
};