#pragma once

#include "fakeDatabaseConnection.h"
#include "IDatabaseConnection.h"
#include <string>

class FakeDatabaseController : public IDatabaseController
{
public:
    bool isDatabaseOpened() const override { return true; }
    std::string getOpenedDatabaseName() const override { return std::string(""); }
    const IDatabaseConnection &getDatabaseConnection() const override { return connection; }
    void openDatabase(const std::string &databaseName) override {}
    void closeDatabase() override {}
private:
    FakeDatabaseConnection connection;
};