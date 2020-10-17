#include "databaseVersionStorage.h"
#include "sqliteOperationFactory.h"

using namespace std;

DatabaseVersionStorage::DatabaseVersionStorage(const DatabaseConnection &connection, 
                                               unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) :  
                         make_unique<SQLiteOperationFactory>()}
{
}

boost::optional<Version> DatabaseVersionStorage::getVersion() 
{
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT name FROM Version");
    if (operation->execute()) {
        if (operation->getRow()) {
            Version retVal(operation->getStringValue(0));
            operation->close();
            return retVal;
        }
        else {
            
            operation->close();
            return {};
        }
    }
    else {
        lastError = operation->getLastError();
        return {};
    }
}


