#pragma once

#include "operationBase.h"
#include <string>
#include <vector>

class SQLiteSelectOperation : public OperationBase
{
public:
    SQLiteSelectOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>());
    bool execute() override;
    sqlite3_stmt *getStatement() const;
private:
    sqlite3_stmt *stmt;
};