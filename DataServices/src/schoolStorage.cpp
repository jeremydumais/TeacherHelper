#include "schoolStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

SchoolStorage::SchoolStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<School> SchoolStorage::getAllItems()
{
    int i =1;
    list<School> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT school.id, school.name, city.id, city.name FROM school "
        "INNER JOIN city ON city.id = school.city_id "
        "WHERE school.deleted=0 AND city.deleted=0 ORDER BY school.name, city.name");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            School tempSchool(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                City(
                                    sqlite3_column_int(stmt, 2),
                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3)))));
            retVal.push_back(tempSchool);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
    }
    return retVal;
}

const std::string &SchoolStorage::getLastError() const
{
    return lastError;
}

bool SchoolStorage::insertItem(const School &school)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO school (name, city_id) VALUES(?, ?)",
        vector<string> { school.getName(), to_string(school.getCity().getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool SchoolStorage::updateItem(const School &school)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE school SET name = ?, city_id = ? WHERE id = ?",
        vector<string> { school.getName(),
            to_string(school.getCity().getId()),
            to_string(school.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool SchoolStorage::deleteItem(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE school SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}
