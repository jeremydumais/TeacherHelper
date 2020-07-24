#include "subjectStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

SubjectStorage::SubjectStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Subject> SubjectStorage::getAllItems()
{
    int i =1;
    list<Subject> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, name, isdefault FROM subject ORDER BY name;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Subject tempSubject(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                static_cast<bool>(sqlite3_column_int(stmt, 2)));
            retVal.push_back(tempSubject);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
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
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO subject (name, isdefault) VALUES(?, ?)",
        vector<string> { boost::trim_copy(subject.getName()), subject.getIsDefault() ? "1" : "0" });
    if (!operation.execute()) {
        lastError = operation.getLastError();
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
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE subject SET name = ?, isdefault = ? WHERE id = ?",
        vector<string> { boost::trim_copy(subject.getName()),
                         subject.getIsDefault() ? "1" : "0",
                         to_string(subject.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

QueryResult SubjectStorage::deleteItem(size_t id)
{
    SQLiteDeleteOperation operation(*connection, 
        "DELETE FROM subject WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
    }
    return operation.getExtendedResultInfo();
}

bool SubjectStorage::updateAllRowsToRemoveDefault(size_t currentSubjectId)
{
    SQLiteSelectOperation operationCheckDefault(*connection, 
    "SELECT COUNT() FROM subject WHERE isdefault=1 and id<>?",
    vector<string> { to_string(currentSubjectId) });
    if (operationCheckDefault.execute()) {
        sqlite3_stmt *stmt = operationCheckDefault.getStatement();
        int result = sqlite3_step(stmt);
        if (result == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            SQLiteUpdateOperation operationUpdateDefault(*connection, 
                "UPDATE subject SET isdefault = 0",
                vector<string>());
            if (!operationUpdateDefault.execute()) {
                lastError = operationUpdateDefault.getLastError();
                return false;
            }
        }
    }
    else {
        lastError = operationCheckDefault.getLastError();
        return false;
    }
    return true;
}
