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
    : ManagementItemStorageBase<City>(connection, move(operationFactory))
{
}

std::string CityStorage::getSelectCommand() const
{
    return "SELECT id, name FROM city ORDER BY name;";
}

City CityStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return City(record.getIntValue(0),
                record.getStringValue(1));
}

std::string CityStorage::getInsertCommand() const
{
    return "INSERT INTO city (name) VALUES(?)";
}

std::vector<std::string> CityStorage::getInsertValues(const City &item) const
{
    return { boost::trim_copy(item.getName()) };
}

std::string CityStorage::getUpdateCommand() const
{
    return "UPDATE city SET name = ? WHERE id = ?";
}

std::vector<std::string> CityStorage::getUpdateValues(const City &item) const
{
    return { boost::trim_copy(item.getName()),
             to_string(item.getId()) 
           };
}
std::string CityStorage::getDeleteCommand() const
{
    return "DELETE FROM city WHERE id = ?";
}

std::vector<std::string> CityStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

std::string CityStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT COUNT(id) FROM school WHERE city_id = ?";
}

std::vector<std::string> CityStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id) };
}