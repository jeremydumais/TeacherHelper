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
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<Student> StudentStorage::getAllItems()
{
    list<Student> retVal;
    auto operation = operationFactory->createSelectOperation(*connection,
        "SELECT id, firstname, lastname, comments FROM student ORDER BY lastname, firstname;");
    if (operation->execute()) {
        while (operation->getRow()) {
            Student tempStudent(operation->getIntValue(0),
                                operation->getStringValue(1),
                                operation->getStringValue(2),
                                operation->getStringValue(3));
            retVal.push_back(tempStudent);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
    }
    return retVal;
}

const std::string &StudentStorage::getLastError() const
{
    return lastError;
}

bool StudentStorage::insertItem(const Student &student)
{
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO student (firstname, lastname, comments) VALUES(?, ?, ?)",
        vector<string> { student.getFirstName(), student.getLastName() , student.getComments()});
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::updateItem(const Student &student)
{
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE student SET firstname = ?, lastname = ?, comments = ? WHERE id = ?",
        vector<string> { student.getFirstName(),
            student.getLastName(),
            student.getComments(),
            to_string(student.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

QueryResult StudentStorage::deleteItem(size_t id)
{
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM student WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}
