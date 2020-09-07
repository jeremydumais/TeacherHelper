#include "cityStorage.h"
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

CityStorage::CityStorage(const DatabaseConnection &connection, 
                         unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<City> CityStorage::getAllItems()
{
    int i =1;
    list<City> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT id, name FROM city ORDER BY name;");
    if (operation->execute()) {
        while (operation->getRow()) {
            City tempCity(operation->getIntValue(0),
                          operation->getStringValue(1));
            retVal.push_back(tempCity);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
    }
    return retVal;
}

const std::string &CityStorage::getLastError() const
{
    return lastError;
}

bool CityStorage::insertItem(const City &city)
{
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO city (name) VALUES(?)",
        vector<string> { boost::trim_copy(city.getName()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

bool CityStorage::updateItem(const City &city)
{
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE city SET name = ? WHERE id = ?",
        vector<string> { boost::trim_copy(city.getName()),
            to_string(city.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

QueryResult CityStorage::deleteItem(size_t id)
{
    //Ensure the the record is not user as a foreign key in another table
    
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM city WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}
