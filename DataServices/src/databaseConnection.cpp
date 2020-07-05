#include "databaseConnection.h"
#include "sqliteDDLOperation.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <sstream>
#include <stdexcept>

using namespace std;
using namespace boost;

DatabaseConnection::DatabaseConnection(const std::string &dbName)
    : dbName(dbName),
      db(nullptr)
{
    if ( trim_copy(dbName).empty()) {
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
        stringstream ss;
        ss << "The database " << dbName << " does not exist.";
        throw runtime_error(ss.str());
    }

    int connection_result = sqlite3_open(dbName.c_str(), &db);
    if (connection_result != 0) {
        stringstream ss;
        ss << "Cannot open database " << dbName << ". sqlite3_errmsg(db)";
        throw runtime_error(ss.str());
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
        stringstream ss;
        ss << "Cannot create database " << dbName << ". sqlite3_errmsg(db)";
        throw runtime_error(ss.str());
    }
    vector<string> tableCreationInstructions {
        "CREATE TABLE school(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, city varchar(50) NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE class(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, school_id INTEGER NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0, FOREIGN KEY(school_id) REFERENCES school(id))",
        "CREATE TABLE student(id INTEGER PRIMARY KEY AUTOINCREMENT, firstname varchar(30) NOT NULL, lastname varchar(30) NOT NULL, deleted BOOLEAN NOT NULL DEFAULT 0)"
    };
    for(const auto& instruction : tableCreationInstructions) {
        SQLiteDDLOperation operationCreateTableSchool(*this, instruction);
        if (!operationCreateTableSchool.execute()) {
            throw runtime_error(operationCreateTableSchool.getLastError());
        }
    }
}
