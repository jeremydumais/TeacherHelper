#include "databaseConnection.h"
#include "sqliteDDLOperation.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <fmt/format.h>
#include <stdexcept>

using namespace std;
using namespace boost;

DatabaseConnection::DatabaseConnection(const std::string &dbName)
    : dbName(dbName),
      db(nullptr)
{
    if (trim_copy(dbName).empty()) {
        throw invalid_argument("dbName cannot be null or empty.");
    }
}

DatabaseConnection::~DatabaseConnection()
{
    sqlite3_close(db);
}

const std::string DatabaseConnection::getDbName() const
{
    return dbName;
}

sqlite3 *DatabaseConnection::getConnectionPtr() const
{
    return db;
}

void DatabaseConnection::open()
{
    if (!filesystem::exists(dbName)) {
        throw runtime_error(fmt::format("The database {0} does not exist.", dbName));
    }

    int connection_result = sqlite3_open(dbName.c_str(), &db);
    if (connection_result != 0) {
        throw runtime_error(fmt::format("Cannot open database {0}. sqlite3_errmsg(db)", dbName));
    }
}

void DatabaseConnection::close()
{
    sqlite3_close(db);
    db = nullptr;
}

void DatabaseConnection::create()
{
    int connection_result = sqlite3_open_v2(dbName.c_str(), 
                                            &db, 
                                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                                            nullptr);
    if (connection_result != 0) {
        throw runtime_error(fmt::format("Cannot create database {0}. sqlite3_errmsg(db)", dbName));
    }
    vector<string> tableCreationInstructions {
        "CREATE TABLE city(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE school(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, city_id INTEGER NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0, FOREIGN KEY(city_id) REFERENCES city(id))",
        "CREATE TABLE class(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, school_id INTEGER NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0, UNIQUE(name, school_id), FOREIGN KEY(school_id) REFERENCES school(id))",
        "CREATE TABLE student(id INTEGER PRIMARY KEY AUTOINCREMENT, firstname varchar(30) NOT NULL, lastname varchar(30) NOT NULL, comments varchar(256), deleted BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE testType(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE subject(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, isdefault BOOLEAN NOT NULL DEFAULT 0, deleted BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE class_student(class_id INTEGER NOT NULL, student_id INTEGER NOT NULL, PRIMARY KEY(class_id, student_id), FOREIGN KEY(class_id) REFERENCES class(id), FOREIGN KEY(student_id) REFERENCES student(id))"
    };
    for(const auto& instruction : tableCreationInstructions) {
        SQLiteDDLOperation operationCreateTableSchool(*this, instruction);
        if (!operationCreateTableSchool.execute()) {
            throw runtime_error(operationCreateTableSchool.getLastError());
        }
    }
}
