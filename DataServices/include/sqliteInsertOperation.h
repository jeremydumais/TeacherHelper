#pragma once

#include "IStorageInsertOperation.h"
#include <string>
#include <vector>

class SQLiteInsertOperation : public IStorageInsertOperation
{
public:
    SQLiteInsertOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args);
    bool execute() override;
};