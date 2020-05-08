#include "databaseConnection.h"
#include <stdexcept>
#include <sstream>

using namespace std;

DatabaseConnection::DatabaseConnection(const std::string dbName)
    : dbName(dbName),
      db(nullptr)
{
}

DatabaseConnection::~DatabaseConnection()
{
    sqlite3_close(db);
}

const std::string DatabaseConnection::getDbName() const
{
    return dbName;
}

void DatabaseConnection::connect()
{
    int connection_result = sqlite3_open(dbName.c_str(), &db);
    if (connection_result) {
        stringstream ss;
        ss << "Cannot open database " << dbName << ". sqlite3_errmsg(db)";
        throw new runtime_error(ss.str());
    }
}

