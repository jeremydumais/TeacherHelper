#pragma once

#include "IStorageSelectOperation.h"
#include <string>
#include <vector>

class SQLiteSelectOperation : public IStorageSelectOperation
{
public:
    SQLiteSelectOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>());
    bool execute() override;
    void close() override;
    sqlite3_stmt *getStatement() const;
    bool getRow() override;
    int getIntValue(int columnNumber) const override;
    std::string getStringValue(int columnNumber) const override;
private:
    sqlite3_stmt *stmt;
};