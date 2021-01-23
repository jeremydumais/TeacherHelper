#include "databaseConnectionFactory.h"
#include "databaseManagementOperations.h"
#include "fileSystemOperations.h"
#include "libraryCurrentVersion.h"
#include "sqliteDatabaseOperations.h"
#include "sqliteOperationFactory.h"
#include <boost/date_time/posix_time/ptime.hpp>
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

bool DatabaseManagementOperations::upgrade(IDatabaseConnection &connection) 
{
    //Extract the database folder path
    string originalDBFileName = connection.getDbName();
    string databaseFolderPath = fileSystemOperations->extractFolderPath(originalDBFileName);
    //Generate backup database filename
    string backupDatabaseFileName = generateBackupDatabaseFileName();
    string backupDatabaseFullPath = fmt::format("{0}/{1}", databaseFolderPath, backupDatabaseFileName);
    //Backup the database file
    onUpgradeProgress(100, fmt::format("Backuping the database {0} to {1}...", originalDBFileName, backupDatabaseFullPath));
    if (!fileSystemOperations->fileCopy(originalDBFileName,
                                        backupDatabaseFullPath)) {
        lastError = fileSystemOperations->getLastError();
        return false;
    }
    onUpgradeProgress(0, "Backup succeed");

    DatabaseVersionStorage databaseVersionStorage(connection);
    auto currentDatabaseVersion = databaseVersionStorage.getVersion();
    bool migrationSucceed { false };
    if (currentDatabaseVersion == Version(1, 0, 0)) {
        migrationSucceed = migrateToV1_1_0(connection, databaseVersionStorage);
    }
    //If an error occurred, restore the backup of the database
    if (!migrationSucceed) {
        connection.close();
        onUpgradeProgress(100, fmt::format("Restoring the database backup {0}...", backupDatabaseFullPath));
        if (!fileSystemOperations->fileCopyWithOverwrite(backupDatabaseFullPath,
                                                            originalDBFileName)) {
            lastError = fileSystemOperations->getLastError();
            onUpgradeProgress(100, fmt::format("Unable to restore the database backup {0} to {1}", backupDatabaseFullPath, originalDBFileName));
        }
        else {
            onUpgradeProgress(100, fmt::format("Restoring the database backup completed!", backupDatabaseFullPath));
        }
        onUpgradeProgress(100, "Upgrade failed!");
        return false;
    }
    return true;
}

std::string DatabaseManagementOperations::generateBackupDatabaseFileName() const
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    string backupDatabaseFileName = fmt::format("teacherdb_v1.0.0_{0}_{1}_{2}_{3}_{4}_{5}.bak",
                                                now.date().year(),
                                                now.date().month().as_number(),
                                                now.date().day().as_number(),
                                                now.time_of_day().hours(),
                                                now.time_of_day().minutes(),
                                                now.time_of_day().seconds());
    return backupDatabaseFileName;
}

bool DatabaseManagementOperations::migrateToV1_1_0(IDatabaseConnection &connection, DatabaseVersionStorage &databaseVersionStorage) 
{
    bool errorOccurred = false;
    onUpgradeProgress(0, "Migration from version 1.0.0 to 1.1.0");
    onUpgradeProgress(0, "Starting the upgrade...");
    //Migrating schema
    vector<string> tableMigrationInstructions {
        "CREATE TABLE version(name varchar(10) PRIMARY KEY)",
        "INSERT INTO version VALUES ('1.0.0')",
        ""
    };
    int percentCompleted { 0 };
    onUpgradeProgress(percentCompleted, fmt::format("{0}%...", percentCompleted));
    for(const auto& instruction : tableMigrationInstructions) {
        auto operationMigrateTable { operationFactory->createDDLOperation(connection, instruction) };
        if (!operationMigrateTable->execute()) {
            lastError = operationMigrateTable->getLastError();
            errorOccurred = true;
            break;
        }
        else {
            percentCompleted += (1.0f / static_cast<float>(tableMigrationInstructions.size())) * 80; 
            onUpgradeProgress(percentCompleted, fmt::format("{0}%...", percentCompleted));
        }
    }
    //80% completed
    if (!errorOccurred && !databaseVersionStorage.updateVersion(Version(1, 1, 0))) {
        lastError = databaseVersionStorage.getLastError();
        onUpgradeProgress(percentCompleted, lastError);
        errorOccurred = true;
    }
    percentCompleted = 100;
    onUpgradeProgress(percentCompleted, fmt::format("{0}%...", percentCompleted));

    if (!errorOccurred) {
        onUpgradeProgress(percentCompleted, "Upgrade completed!");
    }
    return !errorOccurred;
}

