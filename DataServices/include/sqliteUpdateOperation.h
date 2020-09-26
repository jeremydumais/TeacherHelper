#pragma once

#include "IStorageUpdateOperation.h"
#include <string>
#include <vector>

class SQLiteUpdateOperation : public IStorageUpdateOperation
{
public:
    SQLiteUpdateOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
};