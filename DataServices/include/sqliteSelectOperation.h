#pragma once

#include "IStorageSelectOperation.h"
#include <sqlite3.h>
#include <string>
#include <vector>

class SQLiteSelectOperation : public IStorageSelectOperation
{
public:
    SQLiteSelectOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>());
    bool execute() override;
    void close() override;
    bool getRow() override;
    int getIntValue(int columnNumber) const override;
    std::string getStringValue(int columnNumber) const override;
    bool getBoolValue(int columnNumber) const override;
    SQLiteDateTime getDateTime(int columnNumber) const override;
    double getDoubleValue(int columnNumber) const override;
private:
    sqlite3_stmt *stmt;
};