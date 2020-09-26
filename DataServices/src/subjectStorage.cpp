#include "subjectStorage.h"
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

SubjectStorage::SubjectStorage(const DatabaseConnection &connection, 
                               unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<Subject> SubjectStorage::getAllItems()
{
    int i =1;
    list<Subject> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT id, name, isdefault FROM subject ORDER BY name;");
    if (operation->execute()) {
        while (operation->getRow()) {
            Subject tempSubject(operation->getIntValue(0),
                                operation->getStringValue(1),
                                operation->getBoolValue(2));
            retVal.push_back(tempSubject);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
    }
    return retVal;
}

const std::string &SubjectStorage::getLastError() const
{
    return lastError;
}

bool SubjectStorage::insertItem(const Subject &subject)
{
    //If isDefault is true and that there's already a default subject in the database, 
    //then put the old one to false and then proceed with the new one.
    if(subject.getIsDefault()) {
        if (!updateAllRowsToRemoveDefault(subject.getId())) {
            return false;
        }
    }
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO subject (name, isdefault) VALUES(?, ?)",
        vector<string> { boost::trim_copy(subject.getName()), subject.getIsDefault() ? "1" : "0" });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

bool SubjectStorage::updateItem(const Subject &subject)
{
    //If isDefault is true and that there's already a default subject in the database, 
    //then put the old one to false and then proceed with the new one.
    if(subject.getIsDefault()) {
        if (!updateAllRowsToRemoveDefault(subject.getId())) {
            return false;
        }
    }
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE subject SET name = ?, isdefault = ? WHERE id = ?",
        vector<string> { boost::trim_copy(subject.getName()),
                         subject.getIsDefault() ? "1" : "0",
                         to_string(subject.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

QueryResult SubjectStorage::deleteItem(size_t id)
{
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM subject WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}

bool SubjectStorage::updateAllRowsToRemoveDefault(size_t currentSubjectId)
{
    auto operationCheckDefault = operationFactory->createSelectOperation(*connection, 
        "SELECT COUNT() FROM subject WHERE isdefault=1 and id<>?",
    vector<string> { to_string(currentSubjectId) });
    if (operationCheckDefault->execute()) {
        if (operationCheckDefault->getRow() && operationCheckDefault->getIntValue(0) > 0) {
            auto operationUpdateDefault = operationFactory->createUpdateOperation(*connection, 
                "UPDATE subject SET isdefault = 0",
                vector<string>());
            if (!operationUpdateDefault->execute()) {
                lastError = operationUpdateDefault->getLastError();
                operationCheckDefault->close();
                return false;
            }
        }
        operationCheckDefault->close();
    }
    else {
        lastError = operationCheckDefault->getLastError();
        return false;
    }
    return true;
}
