#include "studentStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteUpdateOperation.h"
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <iostream>
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
    int result {0};
    char *zErrMsg {0};
    sqlite3_stmt *stmt;
    stringstream ss;
    ss << "SELECT id, firstname, lastname FROM student WHERE deleted=0 ORDER BY lastname, firstname;";
    
    result = sqlite3_prepare_v2(connection->getConnectionPtr(), 
                          ss.str().c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);
    if(result != SQLITE_OK) {
      lastError = string(zErrMsg);
      sqlite3_free(zErrMsg);
    }

    result = sqlite3_step(stmt);
    while (result == SQLITE_ROW) {
        Student tempStudent(sqlite3_column_int(stmt, 0),
                            reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                            reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))));
        retVal.push_back(tempStudent);
        result = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);

    return retVal;
}

const std::string &StudentStorage::getLastError() const
{
    return lastError;
}

bool StudentStorage::insertStudent(const Student &student)
{
    stringstream ss;
    ss << "INSERT INTO student (firstname, lastname) VALUES(" << 
        "'" << student.getFirstName() << "', "  <<
        "'" << student.getLastName() << "')";
    SQLiteInsertOperation operation(*connection, ss.str());
    if (!operation.Execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::updateStudent(const Student &student)
{
    stringstream ss;
    ss << "UPDATE student SET " << 
        "firstname = '" << student.getFirstName() << "', "  <<
        "lastname = '" << student.getLastName() << "'" <<
        " WHERE id = ?";
    SQLiteUpdateOperation operation(*connection, 
        ss.str(), 
        vector<string> { to_string(student.getId()) });
    if (!operation.Execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

bool StudentStorage::deleteStudent(size_t id)
{
    int result {0};
    sqlite3_stmt *stmt;
    stringstream ss;
    ss << "DELETE FROM student WHERE id = ?";
    result = sqlite3_prepare_v2(connection->getConnectionPtr(), 
                          ss.str().c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);
    if(result != SQLITE_OK) {
        lastError = sqlite3_errmsg(connection->getConnectionPtr());
        return false;
    }
    
    result = sqlite3_bind_int(stmt, 1, id);
    if(result != SQLITE_OK) {
        lastError = sqlite3_errmsg(connection->getConnectionPtr());
        sqlite3_finalize(stmt);
        return false;
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        lastError = sqlite3_errmsg(connection->getConnectionPtr());
        sqlite3_finalize(stmt);
        return false;
    }

    return true;
}
