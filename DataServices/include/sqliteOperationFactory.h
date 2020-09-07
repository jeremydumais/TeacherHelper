#pragma once

#include "IStorageOperationFactory.h"

class SQLiteOperationFactory : public IStorageOperationFactory
{
public:
    std::unique_ptr<IStorageUpdateOperation> createUpateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) override;
};