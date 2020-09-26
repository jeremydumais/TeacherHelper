#pragma once

#include "IStorageOperationFactory.h"

class SQLiteOperationFactory : public IStorageOperationFactory
{
public:
    std::unique_ptr<IStorageSelectOperation> createSelectOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args = std::vector<std::string>()) override;
    std::unique_ptr<IStorageInsertOperation> createInsertOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
    std::unique_ptr<IStorageUpdateOperation> createUpdateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
    std::unique_ptr<IStorageDeleteOperation> createDeleteOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
};