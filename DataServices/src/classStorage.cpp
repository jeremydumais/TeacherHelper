#include "classStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

ClassStorage::ClassStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Class> ClassStorage::getAllItems()
{
    int i =1;
    list<Class> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT class.id, class.name, school.id, school.name, city.id, city.name " 
        "FROM class "
        "INNER JOIN school ON school.id = class.school_id " 
        "INNER JOIN city ON city.id = school.city_id " 
        "WHERE class.deleted = 0 AND school.deleted=0 AND city.deleted = 0 ORDER BY class.name, school.name");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Class tempClass(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                School(
                                  sqlite3_column_int(stmt, 2),
                                  reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3))),
                                  City(
                                    sqlite3_column_int(stmt, 4),
                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 5))))));
            retVal.push_back(tempClass);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
    }
    return retVal;
}

const std::string &ClassStorage::getLastError() const
{
    return lastError;
}

bool ClassStorage::insertItem(const Class &p_class)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO class (name, school_id) VALUES(?, ?)",
        vector<string> { p_class.getName(), to_string(p_class.getSchool().getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool ClassStorage::updateItem(const Class &p_class)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE class SET name = ?, school_id = ? WHERE id = ?",
        vector<string> { p_class.getName(),
            to_string(p_class.getSchool().getId()),
            to_string(p_class.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool ClassStorage::deleteItem(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE class SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}
