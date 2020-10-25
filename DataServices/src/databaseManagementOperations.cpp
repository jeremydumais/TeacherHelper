#include "databaseConnectionFactory.h"
#include "databaseManagementOperations.h"
#include "fileSystemOperations.h"
#include "sqliteDatabaseOperations.h"
#include "sqliteOperationFactory.h"
#include <fmt/format.h>
#include <stdexcept>

using namespace std;


DatabaseManagementOperations::DatabaseManagementOperations(std::shared_ptr<IFileSystemOperations> fileSystemOperations,
                                                           std::shared_ptr<IDatabaseOperations> databaseOperations,
                                                           std::unique_ptr<IDatabaseConnectionFactory> connectionFactory,
                                                           std::shared_ptr<IStorageOperationFactory> operationFactory)
    : lastError {""},
      fileSystemOperations { fileSystemOperations ? 
                             move(fileSystemOperations) :  
                             make_shared<FileSystemOperations>() },
      databaseOperations { databaseOperations ? 
                           move(databaseOperations) :  
                           make_shared<SQLiteDatabaseOperations>() },
      connectionFactory { connectionFactory ? 
                          move(connectionFactory) : 
                          make_unique<DatabaseConnectionFactory>() },
      operationFactory { operationFactory ?
                         move(operationFactory) : 
                         make_shared<SQLiteOperationFactory>() }   
{
}


const std::string &DatabaseManagementOperations::getLastError() const 
{
    return lastError;
}

bool DatabaseManagementOperations::exist(const std::string &databaseName) const 
{
    return fileSystemOperations->fileExists(databaseName);
}

bool DatabaseManagementOperations::create(const string &databaseName)
{
    auto connection { connectionFactory->createNewConnection(databaseName) };
    try {
        connection->openForCreation();
    }
    catch (runtime_error &err) {
        lastError = fmt::format("Cannot create database {0}. sqlite3_errmsg(db)", databaseName);
        return false;
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
        auto operationCreateTableSchool { operationFactory->createDDLOperation(*connection.get(), instruction) };
        if (!operationCreateTableSchool->execute()) {
            lastError = operationCreateTableSchool->getLastError();
            return false;
        }
    }
    return true;
}
