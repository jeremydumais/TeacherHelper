#pragma once

#include "databaseConnection.h"
#include "IStorageDeleteOperation.h"
#include "IStorageInsertOperation.h"
#include "IStorageSelectOperation.h"
#include "IStorageUpdateOperation.h"
#include <vector>
#include <memory>

class IStorageOperationFactory
{
public:
    virtual ~IStorageOperationFactory() {};
    virtual std::unique_ptr<IStorageSelectOperation> createSelectOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args = std::vector<std::string>()) = 0;
    virtual std::unique_ptr<IStorageInsertOperation> createInsertOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) = 0;
    virtual std::unique_ptr<IStorageUpdateOperation> createUpdateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) = 0;
    virtual std::unique_ptr<IStorageDeleteOperation> createDeleteOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) = 0;
};