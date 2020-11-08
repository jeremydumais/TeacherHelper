#include "databaseConnection.h"
#include "databaseController.h"
#include "databaseManagementOperations.h"
#include "libraryCurrentVersion.h"
#include <memory>
#include <stdexcept>

using namespace std;


DatabaseController::DatabaseController(unique_ptr<IDatabaseConnection> databaseConnection,
                                       unique_ptr<IDatabaseManagementOperations> databaseManagementOperations,
                                       unique_ptr<DatabaseVersionStorage> databaseVersionStorage) 
    : lastError {""},
      databaseConnection { databaseConnection ? 
                           move(databaseConnection) : 
                           nullptr },
      databaseManagementOperations { databaseManagementOperations ? 
                                     move(databaseManagementOperations) : 
                                     make_unique<DatabaseManagementOperations>() },
      databaseVersionStorage { databaseVersionStorage ? 
                               move(databaseVersionStorage) :
                               nullptr }                  
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

boost::optional<Version> DatabaseController::getVersion() 
{
    auto version = databaseVersionStorage->getVersion();
    if (!version.has_value()) {
        lastError = databaseVersionStorage->getLastError();
    }
    return version;
}

bool DatabaseController::isDatabaseUpgradeRequired() const 
{
    //Get current database version
    auto currentDatabaseVersion = databaseVersionStorage->getVersion();
    auto currentStorageLibraryVersion = LibraryCurrentVersion::getInstance();
    if (currentStorageLibraryVersion > currentDatabaseVersion) {
        if (currentDatabaseVersion == Version(1, 0, 0) && currentStorageLibraryVersion == Version(1, 1, 0)) {
            return true;
        }
    }
    return false;
}

void DatabaseController::openDatabase(const string &databaseName) 
{
    if (!databaseConnection)
        databaseConnection = make_unique<DatabaseConnection>(databaseName);
    databaseConnection->open();
    databaseVersionStorage = make_unique<DatabaseVersionStorage>(*databaseConnection.get());
}

void DatabaseController::closeDatabase() 
{
    if (!databaseConnection)
        throw runtime_error("No connection has been supplied.");
    if (!databaseConnection->isOpened())
        throw runtime_error("Cannot close an already closed database.");
    
    databaseConnection->close();
    databaseConnection.reset();
    databaseVersionStorage.reset();
}

bool DatabaseController::createDatabase(const string &databaseName) 
{
    bool createResult = databaseManagementOperations->create(databaseName);
    if (!createResult) {
        lastError = databaseManagementOperations->getLastError();
    }
    return createResult;
}

