#include "sqliteDDLOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteDDLOperation::SQLiteDDLOperation(const DatabaseConnection &connection, 
                                             const std::string &query)
    : OperationBase(connection, query)
{
}

bool SQLiteDDLOperation::execute()
{
    int result {0};
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(dbConnection->getConnectionPtr(), 
                          query.c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);

    if(result != SQLITE_OK) {
        lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
        return false;
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
        sqlite3_finalize(stmt);
        return false;
    }

    return true;
}
