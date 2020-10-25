#pragma once

#include "IStorageDDLOperation.h"
#include <string>
#include <vector>

class SQLiteDDLOperation : public IStorageDDLOperation
{
public:
    SQLiteDDLOperation(const IDatabaseConnection &connection, 
                       const std::string &query,
                       const std::vector<std::string> &args = {});
    bool execute() override;
};