#pragma once

#include "operationBase.h" 

class IStorageDDLOperation : public OperationBase
{
public:
    IStorageDDLOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : OperationBase(connection, query, args) {}
    virtual ~IStorageDDLOperation() {};
};