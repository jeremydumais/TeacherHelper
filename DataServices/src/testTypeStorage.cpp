#include "testTypeStorage.h"
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

TestTypeStorage::TestTypeStorage(const IDatabaseConnection &connection, 
                         unique_ptr<IStorageOperationFactory> operationFactory)
    : ManagementItemStorageBase<TestType>(connection, move(operationFactory))
{
}

std::string TestTypeStorage::getSelectCommand() const
{
    return "SELECT id, name FROM testType ORDER BY name;";
}

TestType TestTypeStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return TestType(record.getIntValue(0),
                    record.getStringValue(1));
}

std::string TestTypeStorage::getInsertCommand() const
{
    return "INSERT INTO testType (name) VALUES(?)";
}

std::vector<std::string> TestTypeStorage::getInsertValues(const TestType &item) const
{
    return { boost::trim_copy(item.getName()) };
}

std::string TestTypeStorage::getUpdateCommand() const
{
    return "UPDATE testType SET name = ? WHERE id = ?";
}

std::vector<std::string> TestTypeStorage::getUpdateValues(const TestType &item) const
{
    return { boost::trim_copy(item.getName()),
             to_string(item.getId()) };
}
std::string TestTypeStorage::getDeleteCommand() const
{
    return "DELETE FROM testType WHERE id = ?";
}

std::vector<std::string> TestTypeStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

std::string TestTypeStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT COUNT(id) FROM assessment WHERE testType_id = ?";
}

std::vector<std::string> TestTypeStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id) };
}