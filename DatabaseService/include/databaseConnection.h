#pragma once

#include <string>
#include <sqlite3.h>

class DatabaseConnection
{
public:
    DatabaseConnection(const std::string &dbName);
    virtual ~DatabaseConnection();
    const std::string getDbName() const;
    void connect();
private:
    std::string dbName;
	sqlite3 *db;
};