#include "databaseController.h"

using namespace std;


DatabaseController::DatabaseController(string dbName,
                                       unique_ptr<IDatabaseConnection> databaseConnection) 
    : databaseConnection { databaseConnection ? 
                           move(databaseConnection) : 
                           unique_ptr<IDatabaseConnection>(make_unique<DatabaseConnection>(dbName))}
{
}

const std::string& DatabaseController::getDatabaseName() const
{
    return databaseConnection->getDbName();
}

void DatabaseController::openDatabase() 
{
    databaseConnection->open();
}

void DatabaseController::closeDatabase() 
{
    databaseConnection->close();
}

