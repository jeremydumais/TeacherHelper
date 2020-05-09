#include "databaseConnection.h"
#include <stdexcept>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost;

DatabaseConnection::DatabaseConnection(const std::string &dbName)
    : dbName(dbName),
      db(nullptr)
{
    if ( trim_copy(dbName) == "") {
        throw invalid_argument("dbName cannot be null or empty");
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

void DatabaseConnection::connect()
{
    if (!filesystem::exists(dbName)) {
        stringstream ss;
        ss << "The database " << dbName << " does not exist.";
        throw runtime_error(ss.str());
    }
    else {
        int connection_result = sqlite3_open(dbName.c_str(), &db);
        if (connection_result) {
            stringstream ss;
            ss << "Cannot open database " << dbName << ". sqlite3_errmsg(db)";
            throw runtime_error(ss.str());
        }
    }
}

