#include "testTypeStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

TestTypeStorage::TestTypeStorage(const DatabaseConnection &connection, 
                         unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<TestType> TestTypeStorage::getAllItems()
{
    int i =1;
    list<TestType> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT id, name FROM testType ORDER BY name;");
    if (operation->execute()) {
        while (operation->getRow()) {
            TestType tempTestType(operation->getIntValue(0),
                                operation->getStringValue(1));
            retVal.push_back(tempTestType);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
    }
    return retVal;
}

const std::string &TestTypeStorage::getLastError() const
{
    return lastError;
}

bool TestTypeStorage::insertItem(const TestType &testType)
{
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO testType (name) VALUES(?)",
        vector<string> { boost::trim_copy(testType.getName()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

bool TestTypeStorage::updateItem(const TestType &testType)
{
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE testType SET name = ? WHERE id = ?",
        vector<string> { boost::trim_copy(testType.getName()),
            to_string(testType.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

QueryResult TestTypeStorage::deleteItem(size_t id)
{
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM testType WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}
