#pragma once

#include "fakeDatabaseConnection.h"
#include "IDatabaseConnection.h"
#include "version.h"
#include <boost/optional.hpp>
#include <string>

class FakeDatabaseController : public IDatabaseController
{
public:
    bool isDatabaseOpened() const override { return true; }
    std::string getOpenedDatabaseName() const override { return databaseName; }
    const IDatabaseConnection &getDatabaseConnection() const override { return connection; }
    bool isDatabaseExist(const std::string &databaseName) const override { return true; }
    const std::string &getLastError() const override { return lastError; };
    boost::optional<Version> getVersion() override { return Version(1, 0, 0); }
    bool isDatabaseUpgradeRequired() const override { return false; };
    bool upgrade() { return false; }
    void openDatabase(const std::string &databaseName) override {}
    void closeDatabase() override {}
    bool createDatabase(const std::string &databaseName) override { return true; }
private:
    FakeDatabaseConnection connection;
    std::string databaseName;
    std::string lastError;
};