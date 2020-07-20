#include "testTypeStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

TestTypeStorage::TestTypeStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<TestType> TestTypeStorage::getAllItems()
{
    int i =1;
    list<TestType> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, name FROM testType WHERE deleted=0 ORDER BY name;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            TestType tempTestType(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))));
            retVal.push_back(tempTestType);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
    }
    return retVal;
}

const std::string &TestTypeStorage::getLastError() const
{
    return lastError;
}

bool TestTypeStorage::insertItem(const TestType &testType)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO testType (name) VALUES(?)",
        vector<string> { boost::trim_copy(testType.getName()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool TestTypeStorage::updateItem(const TestType &testType)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE testType SET name = ? WHERE id = ?",
        vector<string> { boost::trim_copy(testType.getName()),
            to_string(testType.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool TestTypeStorage::deleteItem(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE testType SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}
