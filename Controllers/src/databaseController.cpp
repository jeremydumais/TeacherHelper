#include "databaseController.h"

using namespace std;


DatabaseController::DatabaseController(unique_ptr<IDatabaseConnection> databaseConnection) 
    : databaseConnection { databaseConnection ? 
                           move(databaseConnection) : 
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
