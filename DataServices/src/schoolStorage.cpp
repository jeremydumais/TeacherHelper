#include "schoolStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

SchoolStorage::SchoolStorage(const DatabaseConnection &connection, 
                             unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<School> SchoolStorage::getAllItems()
{
    int i =1;
    list<School> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT school.id, school.name, city.id, city.name FROM school "
        "INNER JOIN city ON city.id = school.city_id "
        "ORDER BY school.name, city.name");
    if (operation->execute()) {
        while (operation->getRow()) {
            School tempSchool(operation->getIntValue(0),
                              operation->getStringValue(1),
                              City(
                                operation->getIntValue(2),
                                operation->getStringValue(3)));
            retVal.push_back(tempSchool);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
    }
    return retVal;
}

const std::string &SchoolStorage::getLastError() const
{
    return lastError;
}

bool SchoolStorage::insertItem(const School &school)
{
    auto operation = operationFactory->createInsertOperation(*connection,
        "INSERT INTO school (name, city_id) VALUES(?, ?)",
        vector<string> { school.getName(), to_string(school.getCity().getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

bool SchoolStorage::updateItem(const School &school)
{
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE school SET name = ?, city_id = ? WHERE id = ?",
        vector<string> { school.getName(),
            to_string(school.getCity().getId()),
            to_string(school.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

QueryResult SchoolStorage::deleteItem(size_t id)
{
    auto operation = operationFactory->createDeleteOperation(*connection,
        "DELETE FROM school WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}
