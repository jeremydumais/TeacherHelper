#pragma once

#include "databaseConnection.h"
#include "IStorageUpdateOperation.h"
#include <vector>
#include <memory>

class IStorageOperationFactory
{
public:
    virtual ~IStorageOperationFactory() {};
    virtual std::unique_ptr<IStorageUpdateOperation> createUpateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) = 0;
};