#include "sqliteDeleteOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteDeleteOperation::SQLiteDeleteOperation(const DatabaseConnection &connection, 
                                             const string &query,
                                             const vector<string> &args)
    : OperationBase(connection, query, args)
{
}

bool SQLiteDeleteOperation::execute()
{
    int result {0};
    sqlite3_stmt *stmt;
    result = sqlite3_prepare_v2(dbConnection->getConnectionPtr(), 
                          query.c_str(), 
                          -1, 
                          &stmt, 
                          nullptr);

    if(result != SQLITE_OK) {
        extendedResultInfo = QueryResult::ERROR;
        lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
        return false;
    }
    
    for(int i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, i, (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            extendedResultInfo = QueryResult::ERROR;
            lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
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
        lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
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
