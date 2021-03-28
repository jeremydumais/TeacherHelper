#pragma once

#include "operationBase.h" 

class IStorageInsertOperation : public OperationBase
{
public:
    IStorageInsertOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : OperationBase(connection, query, args) {}
    virtual ~IStorageInsertOperation() {};
};