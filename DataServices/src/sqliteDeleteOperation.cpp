#include "sqliteDeleteOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteDeleteOperation::SQLiteDeleteOperation(const IDatabaseConnection &connection, 
                                             const string &query,
                                             const vector<string> &args)
    : IStorageDeleteOperation(connection, query, args)
{
}

bool SQLiteDeleteOperation::execute()
{
    int result {0};
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(static_cast<sqlite3*>(dbConnection->getConnectionPtr()), 
                          query.c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);

    if(result != SQLITE_OK) {
        extendedResultInfo = QueryResult::ERROR;
        lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
        return false;
    }
    
    for(size_t i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, static_cast<int>(i), (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            extendedResultInfo = QueryResult::ERROR;
            lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
            sqlite3_finalize(stmt);
            return false;
        }
    }

    result = sqlite3_step(stmt);
    if(result != SQLITE_DONE) {
        if (result == SQLITE_CONSTRAINT) {
            extendedResultInfo = QueryResult::CONSTRAINTERROR;
        } else {
            extendedResultInfo = QueryResult::ERROR;
        }
        lastError = sqlite3_errmsg(static_cast<sqlite3*>(dbConnection->getConnectionPtr()));
        sqlite3_finalize(stmt);
        return false;
    }
    extendedResultInfo = QueryResult::OK;

    return true;
}


QueryResult SQLiteDeleteOperation::getExtendedResultInfo() const
{
    return extendedResultInfo;
}
