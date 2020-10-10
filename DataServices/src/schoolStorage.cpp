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
    : ManagementItemStorageBase<School>(connection, move(operationFactory))
{
}

std::string SchoolStorage::getSelectCommand() const
{
    return "SELECT school.id, school.name, city.id, city.name FROM school "
           "INNER JOIN city ON city.id = school.city_id "
           "ORDER BY school.name, city.name";
}

School SchoolStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return School (record.getIntValue(0),
                   record.getStringValue(1),
                   City(record.getIntValue(2),
                        record.getStringValue(3)));
}

std::string SchoolStorage::getInsertCommand() const
{
    return "INSERT INTO school (name, city_id) VALUES(?, ?)";
}

std::vector<std::string> SchoolStorage::getInsertValues(const School &item) const
{
    return { item.getName(), to_string(item.getCity().getId()) };
}

std::string SchoolStorage::getUpdateCommand() const
{
    return "UPDATE school SET name = ?, city_id = ? WHERE id = ?";
}

std::vector<std::string> SchoolStorage::getUpdateValues(const School &item) const
{
    return { item.getName(),
             to_string(item.getCity().getId()),
             to_string(item.getId()) };
}
std::string SchoolStorage::getDeleteCommand() const
{
    return "DELETE FROM school WHERE id = ?";
}

std::vector<std::string> SchoolStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

std::string SchoolStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT COUNT(id) FROM class WHERE school_id = ?";
}

std::vector<std::string> SchoolStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id) };
}