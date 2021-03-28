#pragma once

#include "IStorageOperationFactory.h"

class SQLiteOperationFactory : public IStorageOperationFactory
{
public:
    std::unique_ptr<IStorageSelectOperation> createSelectOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args = std::vector<std::string>()) override;
    std::unique_ptr<IStorageInsertOperation> createInsertOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
    std::unique_ptr<IStorageUpdateOperation> createUpdateOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
    std::unique_ptr<IStorageDeleteOperation> createDeleteOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
    std::unique_ptr<IStorageDDLOperation> createDDLOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args = std::vector<std::string>()) override;

};