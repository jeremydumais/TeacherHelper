#pragma once

#include <string>
#include <sqlite3.h>

class DatabaseConnection
{
public:
    explicit DatabaseConnection(const std::string &dbName);
    virtual ~DatabaseConnection();
    const std::string getDbName() const;
    sqlite3 *getConnectionPtr() const;
    void open();
    void close();
    void create();
private:
    std::string dbName;
	sqlite3 *db;
};