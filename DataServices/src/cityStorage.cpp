#include "cityStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

CityStorage::CityStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<City> CityStorage::getAllCities()
{
    int i =1;
    list<City> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, name FROM city WHERE deleted=0 ORDER BY name;");
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
    }
    return retVal;
}

const std::string &CityStorage::getLastError() const
{
    return lastError;
}

bool CityStorage::insertCity(const City &city)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO city (name) VALUES(?)",
        vector<string> { city.getName() });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool CityStorage::updateCity(const City &city)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE city SET name = ? WHERE id = ?",
        vector<string> { city.getName(),
            to_string(city.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool CityStorage::deleteCity(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE city SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}
