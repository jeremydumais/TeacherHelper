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

list<School> SchoolStorage::getAllSchools()
{
    int i =1;
    list<School> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, name, city FROM school WHERE deleted=0 ORDER BY name, city;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            School tempSchool(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))));
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

bool SchoolStorage::insertSchool(const School &school)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO school (name, city) VALUES(?, ?)",
        vector<string> { school.getName(), school.getCity() });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool SchoolStorage::updateSchool(const School &school)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE school SET name = ?, city = ? WHERE id = ?",
        vector<string> { school.getName(),
            school.getCity(),
            to_string(school.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool SchoolStorage::deleteSchool(size_t id)
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
