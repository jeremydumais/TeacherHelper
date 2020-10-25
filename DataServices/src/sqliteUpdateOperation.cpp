#include "sqliteUpdateOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteUpdateOperation::SQLiteUpdateOperation(const IDatabaseConnection &connection, 
                                             const string &query,
                                             const vector<string> &args)
    : IStorageUpdateOperation(connection, query, args)
{
}

bool SQLiteUpdateOperation::execute()
{
    int result {0};
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
    
    for(int i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, i, (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
            sqlite3_finalize(stmt);
            return false;
        }
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
        sqlite3_finalize(stmt);
        return false;
    }

    return true;
}
