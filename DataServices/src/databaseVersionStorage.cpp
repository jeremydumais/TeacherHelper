#include "databaseVersionStorage.h"
#include "sqliteOperationFactory.h"

using namespace std;

DatabaseVersionStorage::DatabaseVersionStorage(const IDatabaseConnection &connection, 
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
    //Check if the version table exist, if not it means that it is version 1.0.0
    bool tableExist { false };
    try {
        tableExist = isVersionTableExist();
    }
    catch(runtime_error &err) {
        return {};
    }
    if (!tableExist) {
        return Version(1, 0, 0);
    }

    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT name FROM Version"s);
    if (operation->execute()) {
        if (operation->getRow()) {
            try {
                Version retVal(operation->getStringValue(0));
                operation->close();
                return retVal;
            }
            catch(invalid_argument &err) {
                lastError = err.what();
            }
            catch(out_of_range &err) {
                lastError = err.what();
            }
        }
        else {
            lastError = operation->getLastError();
        }
        operation->close(); 
    }
    else {
        lastError = operation->getLastError();
    }
    return {};
}


bool DatabaseVersionStorage::isVersionTableExist() 
{
    auto operationTableExist = operationFactory->createSelectOperation(*connection, 
        "SELECT name FROM sqlite_master WHERE type='table' AND name='version'"s);
    if (!operationTableExist->execute()) {
        lastError = operationTableExist->getLastError();
        throw runtime_error(lastError);  
    }
    else if (!operationTableExist->getRow()) {
        operationTableExist->close();
        return false;
    }
    else {
        return operationTableExist->getStringValue(0) == "version";
    }
}

const string &DatabaseVersionStorage::getLastError() const {
        return lastError;
}


bool DatabaseVersionStorage::updateVersion(Version version) 
{
    auto operation = operationFactory->createUpdateOperation(*connection, 
            "UPDATE version SET name = ?",
            { version.str() });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}
