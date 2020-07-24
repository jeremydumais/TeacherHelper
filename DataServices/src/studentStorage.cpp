#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "studentStorage.h"
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

StudentStorage::StudentStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Student> StudentStorage::getAllItems()
{
    list<Student> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, firstname, lastname, comments FROM student ORDER BY lastname, firstname;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Student tempStudent(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3))));
            retVal.push_back(tempStudent);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
    }
    return retVal;
}

const std::string &StudentStorage::getLastError() const
{
    return lastError;
}

bool StudentStorage::insertItem(const Student &student)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO student (firstname, lastname, comments) VALUES(?, ?, ?)",
        vector<string> { student.getFirstName(), student.getLastName() , student.getComments()});
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::updateItem(const Student &student)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE student SET firstname = ?, lastname = ?, comments = ? WHERE id = ?",
        vector<string> { student.getFirstName(),
            student.getLastName(),
            student.getComments(),
            to_string(student.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

QueryResult StudentStorage::deleteItem(size_t id)
{
    SQLiteDeleteOperation operation(*connection, 
        "DELETE FROM student WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
    }
    return operation.getExtendedResultInfo();
}
