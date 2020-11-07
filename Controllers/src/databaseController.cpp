#include "databaseConnection.h"
#include "databaseController.h"
#include "databaseManagementOperations.h"
#include <memory>
#include <stdexcept>

using namespace std;


DatabaseController::DatabaseController(unique_ptr<IDatabaseConnection> databaseConnection,
                                       unique_ptr<IDatabaseManagementOperations> databaseManagementOperations) 
    : lastError {""},
      databaseConnection { databaseConnection ? 
                           move(databaseConnection) : 
                           nullptr },
      databaseManagementOperations { databaseManagementOperations ? 
                                     move(databaseManagementOperations) : 
                                     make_unique<DatabaseManagementOperations>() }
{
}

bool DatabaseController::isDatabaseOpened() const
{
    if (databaseConnection) {
        return databaseConnection->isOpened();
    }
    else {
        return false;
    }
}

std::string DatabaseController::getOpenedDatabaseName() const
{
    if (databaseConnection)
        return databaseConnection->getDbName();
    else
        return "";
}

const IDatabaseConnection& DatabaseController::getDatabaseConnection() const
{
    return *databaseConnection.get();
}


bool DatabaseController::isDatabaseExist(const string &databaseName) const 
{
    return databaseManagementOperations->exist(databaseName);
}

const std::string &DatabaseController::getLastError() const
{
    return lastError;
}

void DatabaseController::openDatabase(const string &databaseName) 
{
    if (!databaseConnection)
        databaseConnection = make_unique<DatabaseConnection>(databaseName);
    databaseConnection->open();
}

void DatabaseController::closeDatabase() 
{
    if (!databaseConnection)
        throw runtime_error("No connection has been supplied.");
    if (!databaseConnection->isOpened())
        throw runtime_error("Cannot close an already closed database.");
    
    databaseConnection->close();
    databaseConnection.reset();
}

bool DatabaseController::createDatabase(const string &databaseName) 
{
    bool createResult = databaseManagementOperations->create(databaseName);
    if (!createResult) {
        lastError = databaseManagementOperations->getLastError();
    }
    return createResult;
}

