#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
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

list<Student> StudentStorage::getAllStudents()
{
    list<Student> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT id, firstname, lastname FROM student WHERE deleted=0 ORDER BY lastname, firstname;");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Student tempStudent(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))));
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

bool StudentStorage::insertStudent(const Student &student)
{
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO student (firstname, lastname) VALUES(?, ?)",
        vector<string> { student.getFirstName(), student.getLastName() });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::updateStudent(const Student &student)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE student SET firstname = ?, lastname = ? WHERE id = ?",
        vector<string> { student.getFirstName(),
            student.getLastName(),
            to_string(student.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::deleteStudent(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE student SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}
