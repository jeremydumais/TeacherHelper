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