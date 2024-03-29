#include "sqliteDateTimeFactory.h"
#include "sqliteSelectOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteSelectOperation::SQLiteSelectOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const vector<string> &args)
    : IStorageSelectOperation(connection, query, args),
      stmt(nullptr)
{
}

bool SQLiteSelectOperation::execute()
{
    int result {0};
    char *zErrMsg {nullptr};
    sqlite3_stmt *stmt;
    
    result = sqlite3_prepare_v2(static_cast<sqlite3*>(dbConnection->getConnectionPtr()), 
                          query.c_str(),
                          -1, 
                          &stmt, 
                          nullptr);
    if(result != SQLITE_OK) {
        if (zErrMsg) {
            lastError = string(zErrMsg);
            sqlite3_free(zErrMsg);  
        } else {
            lastError = "Unknown error";
        }
        return false;
    }

    for(size_t i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, static_cast<int>(i), (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
            sqlite3_finalize(stmt);
            return false;
        }
    }

    this->stmt = stmt;
    return true;
}

void SQLiteSelectOperation::close() 
{
    sqlite3_finalize(stmt);
}

bool SQLiteSelectOperation::getRow()
{
    return sqlite3_step(stmt) == SQLITE_ROW;
}

int SQLiteSelectOperation::getIntValue(int columnNumber) const
{
    return sqlite3_column_int(stmt, columnNumber);
}

std::string SQLiteSelectOperation::getStringValue(int columnNumber) const
{
    return reinterpret_cast<const char *>(sqlite3_column_text(stmt, columnNumber));
}

bool SQLiteSelectOperation::getBoolValue(int columnNumber) const
{
    return static_cast<bool>(sqlite3_column_int(stmt, columnNumber));
}

SQLiteDateTime SQLiteSelectOperation::getDateTime(int columnNumber) const 
{
    return SQLiteDateTimeFactory::NewDateTimeFromISOExtended(reinterpret_cast<const char *>(sqlite3_column_text(stmt, columnNumber)));
}

double SQLiteSelectOperation::getDoubleValue(int columnNumber) const
{
    return sqlite3_column_double(stmt, columnNumber);
}

float SQLiteSelectOperation::getFloatValue(int columnNumber) const
{
    return static_cast<float>(sqlite3_column_double(stmt, columnNumber));
}
