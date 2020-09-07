#include "sqliteInsertOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteInsertOperation::SQLiteInsertOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const vector<string> &args)
    : IStorageInsertOperation(connection, query, args)
{
}

bool SQLiteInsertOperation::execute()
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

    for(int i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, i, (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
            sqlite3_finalize(stmt);
            return false;
        }
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
        sqlite3_finalize(stmt);
        return false;
    }

    return true;
}
