#include "databaseConnection.h"
#include "fileSystemOperations.h"
#include "sqliteDatabaseOperations.h"
#include "sqliteDDLOperation.h"
#include "sqliteOperationFactory.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <memory>
#include <stdexcept>

using namespace std;
using namespace boost;

DatabaseConnection::DatabaseConnection(const std::string &dbName,
                                       std::unique_ptr<IFileSystemOperations> fileSystemOperations,
                                       std::unique_ptr<IDatabaseOperations> databaseOperations,
                                       std::unique_ptr<IStorageOperationFactory> operationFactory)
    : dbName(dbName),
      isDBOpened(false),
      fileSystemOperations { fileSystemOperations ? 
                             move(fileSystemOperations) :  
                             make_unique<FileSystemOperations>() },
      databaseOperations { databaseOperations ?
                           move(databaseOperations) : 
                           make_unique<SQLiteDatabaseOperations>() },
      operationFactory { operationFactory ? 
                         move(operationFactory) :  
                         make_unique<SQLiteOperationFactory>()}
{
    if (trim_copy(dbName).empty()) {
        throw invalid_argument("dbName cannot be null or empty.");
    }
}

DatabaseConnection::~DatabaseConnection()
{
    databaseOperations->close();
}

const std::string &DatabaseConnection::getDbName() const
{
    return dbName;
}

void *DatabaseConnection::getConnectionPtr() const
{
    return databaseOperations->getConnectionPtr();
}

void DatabaseConnection::open()
{
    if (!fileSystemOperations->fileExists(dbName)) {
        throw runtime_error(fmt::format("The database {0} does not exist.", dbName));
    }

    int connection_result = databaseOperations->open(dbName.c_str());
    if (connection_result != 0) {
        throw runtime_error(fmt::format("Cannot open database {0}. sqlite3_errmsg(db)", dbName));
    }
    isDBOpened = true;
    //Enabling Foreign Key Support
    auto operation = operationFactory->createDDLOperation(*this, "PRAGMA foreign_keys = ON");
    if (!operation->execute()) {
            throw runtime_error(operation->getLastError());
    }
}

void DatabaseConnection::openForCreation() 
{
    int connection_result = databaseOperations->openForCreation(dbName.c_str());
    if (connection_result != 0) {
        throw runtime_error(fmt::format("Cannot open database {0} for creation. sqlite3_errmsg(db)", dbName));
    }
    isDBOpened = true;
    //Enabling Foreign Key Support
    auto operation = operationFactory->createDDLOperation(*this, "PRAGMA foreign_keys = ON");
    if (!operation->execute()) {
            throw runtime_error(operation->getLastError());
    }
}

bool DatabaseConnection::isOpened() const
{
    return isDBOpened;
}

void DatabaseConnection::close()
{
    databaseOperations->close();
    isDBOpened = false;
}

void DatabaseConnection::create()
{
    /*int connection_result = sqlite3_open_v2(dbName.c_str(), 
                                            &db, 
                                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                                            nullptr);
    if (connection_result != 0) {
        throw runtime_error(fmt::format("Cannot create database {0}. sqlite3_errmsg(db)", dbName));
    }
    vector<string> tableCreationInstructions {
        "CREATE TABLE city(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL)",
        "CREATE TABLE school(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, city_id INTEGER NOT NULL, FOREIGN KEY(city_id) REFERENCES city(id))",
        "CREATE TABLE class(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, school_id INTEGER NOT NULL, UNIQUE(name, school_id), FOREIGN KEY(school_id) REFERENCES school(id))",
        "CREATE TABLE student(id INTEGER PRIMARY KEY AUTOINCREMENT, firstname varchar(30) NOT NULL, lastname varchar(30) NOT NULL, comments varchar(256))",
        "CREATE TABLE testType(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL)",
        "CREATE TABLE subject(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(50) NOT NULL, isdefault BOOLEAN NOT NULL DEFAULT 0)",
        "CREATE TABLE class_student(class_id INTEGER NOT NULL, student_id INTEGER NOT NULL, PRIMARY KEY(class_id, student_id), FOREIGN KEY(class_id) REFERENCES class(id), FOREIGN KEY(student_id) REFERENCES student(id))",
        "CREATE TABLE assessment(id INTEGER PRIMARY KEY AUTOINCREMENT, name varchar(100) NOT NULL, testType_id INTEGER NOT NULL, subject_id INTEGER NOT NULL, class_id INTEGER NOT NULL, date TEXT NOT NULL, FOREIGN KEY(testType_id) REFERENCES testType(id), FOREIGN KEY(subject_id) REFERENCES subject(id), FOREIGN KEY(class_id) REFERENCES class(id))",
        "CREATE TABLE assessmentResult(id INTEGER PRIMARY KEY AUTOINCREMENT, assessment_id INTEGER NOT NULL, student_id INTEGER NOT NULL, result REAL NOT NULL, comments varchar(256), FOREIGN KEY(assessment_id) REFERENCES assessment(id), FOREIGN KEY(student_id) REFERENCES student(id))"
    };
    for(const auto& instruction : tableCreationInstructions) {
        SQLiteDDLOperation operationCreateTableSchool(*this, instruction);
        if (!operationCreateTableSchool.execute()) {
            throw runtime_error(operationCreateTableSchool.getLastError());
        }
    }*/
}
