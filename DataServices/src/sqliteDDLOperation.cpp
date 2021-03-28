#include "sqliteDDLOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteDDLOperation::SQLiteDDLOperation(const IDatabaseConnection &connection, 
                                       const std::string &query,
                                       const std::vector<std::string> &args)
    : IStorageDDLOperation(connection, query, args)
{
}

bool SQLiteDDLOperation::execute()
{
    int result {0};
    lastError = "";
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(static_cast<sqlite3*>(dbConnection->getConnectionPtr()), 
                          query.c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);

    if(result != SQLITE_OK) {
        lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
        return false;
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
        sqlite3_finalize(stmt);
        return false;
    }

    return true;
}
