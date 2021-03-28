#pragma once

#include "operationBase.h" 
#include "queryResult.h"

class IStorageDeleteOperation : public OperationBase
{
public:
    IStorageDeleteOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : OperationBase(connection, query, args) {}
    virtual ~IStorageDeleteOperation() {};
    virtual QueryResult getExtendedResultInfo() const = 0;
};