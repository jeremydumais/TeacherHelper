#pragma once

#include "operationBase.h" 

class IStorageUpdateOperation : public OperationBase
{
public:
    IStorageUpdateOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : OperationBase(connection, query, args) {}
    virtual ~IStorageUpdateOperation() {};
};