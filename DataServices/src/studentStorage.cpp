#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include "studentStorage.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

StudentStorage::StudentStorage(const DatabaseConnection &connection, 
                               unique_ptr<IStorageOperationFactory> operationFactory)
    : ManagementItemStorageBase<Student>(connection, move(operationFactory))
{
}

std::string StudentStorage::getSelectCommand() const
{
    return "SELECT id, firstname, lastname, comments FROM student ORDER BY lastname, firstname;";
}

Student StudentStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return Student(record.getIntValue(0),
                   record.getStringValue(1),
                   record.getStringValue(2),
                   record.getStringValue(3));;
}

std::string StudentStorage::getInsertCommand() const
{
    return "INSERT INTO student (firstname, lastname, comments) VALUES(?, ?, ?)";
}

std::vector<std::string> StudentStorage::getInsertValues(const Student &item) const
{
    return { item.getFirstName(), item.getLastName(), item.getComments()};
}

std::string StudentStorage::getUpdateCommand() const
{
    return "UPDATE student SET firstname = ?, lastname = ?, comments = ? WHERE id = ?";
}

std::vector<std::string> StudentStorage::getUpdateValues(const Student &item) const
{
    return { item.getFirstName(),
             item.getLastName(),
             item.getComments(),
             to_string(item.getId()) };
}
std::string StudentStorage::getDeleteCommand() const
{
    return "DELETE FROM student WHERE id = ?";
}

std::vector<std::string> StudentStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

std::string StudentStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT SUM(sum_count) FROM ("
           "SELECT COUNT(class_id) as sum_count FROM class_student WHERE student_id = ? "
           "UNION ALL "
           "SELECT COUNT(id) as sum_count FROM assessmentResult WHERE student_id = ?)";
}

std::vector<std::string> StudentStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id), to_string(id) };
}
