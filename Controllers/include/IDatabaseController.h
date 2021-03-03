#pragma once

#include "IDatabaseConnection.h"
#include "version.h"
#include <boost/optional.hpp>
#include <boost/signals2.hpp>
#include <string>

class IDatabaseController
{
public:
    virtual ~IDatabaseController() {};
    boost::signals2::signal<void(size_t progress, const std::string &message)> onUpgradeProgress;
    virtual bool isDatabaseOpened() const = 0;
    virtual std::string getOpenedDatabaseName() const = 0;
    virtual const IDatabaseConnection &getDatabaseConnection() const = 0;
    virtual bool isDatabaseExist(const std::string &databaseName) const = 0;
    virtual const std::string &getLastError() const = 0;
    virtual boost::optional<Version> getVersion() = 0;
    virtual bool isDatabaseUpgradeRequired() const = 0;
    virtual bool upgrade() = 0;
    virtual void openDatabase(const std::string &databaseName) = 0;
    virtual void closeDatabase() = 0;
    virtual bool createDatabase(const std::string &databaseName) = 0;
};