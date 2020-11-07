#pragma once

#include "IDatabaseConnection.h"
#include <string>

class IDatabaseController
{
public:
    virtual bool isDatabaseOpened() const = 0;
    virtual std::string getOpenedDatabaseName() const = 0;
    virtual const IDatabaseConnection &getDatabaseConnection() const = 0;
    virtual void openDatabase(const std::string &databaseName) = 0;
    virtual void closeDatabase() = 0;
};