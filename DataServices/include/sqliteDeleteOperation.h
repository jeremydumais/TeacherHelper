#pragma once

#include "IStorageDeleteOperation.h"
#include "queryResult.h"
#include <string>
#include <vector>

class SQLiteDeleteOperation : public IStorageDeleteOperation
{
public:
    SQLiteDeleteOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
    QueryResult getExtendedResultInfo() const override;
private:
    QueryResult extendedResultInfo;
};