#include "assessmentStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

AssessmentStorage::AssessmentStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Assessment> AssessmentStorage::getAllItems()
{
    int i =1;
    list<Assessment> retVal;
    /*SQLiteSelectOperation operation(*connection, 
        "SELECT id, name FROM city ORDER BY name;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            City tempCity(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))));
            retVal.push_back(tempCity);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
    }*/
    return retVal;
}

const std::string &AssessmentStorage::getLastError() const
{
    return lastError;
}

bool AssessmentStorage::insertItem(const Assessment &assessment)
{
    /*SQLiteInsertOperation operation(*connection, 
        "INSERT INTO city (name) VALUES(?)",
        vector<string> { boost::trim_copy(city.getName()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }*/
    return true;
}

bool AssessmentStorage::updateItem(const Assessment &assessment)
{
    /*SQLiteUpdateOperation operation(*connection, 
        "UPDATE city SET name = ? WHERE id = ?",
        vector<string> { boost::trim_copy(city.getName()),
            to_string(city.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }*/
    return true;
}

QueryResult AssessmentStorage::deleteItem(size_t id)
{
    //Ensure the the record is not user as a foreign key in another table

    SQLiteDeleteOperation operation(*connection, 
        "DELETE FROM city WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
    }
    return operation.getExtendedResultInfo();
}
