#include "sqliteSelectOperation.h"
#include <sqlite3.h>

using namespace std;

SQLiteSelectOperation::SQLiteSelectOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const vector<string> &args)
    : OperationBase(connection, query, args)
{
}

bool SQLiteSelectOperation::execute()
{
    int result {0};
    char *zErrMsg {0};
    sqlite3_stmt *stmt;
    
    result = sqlite3_prepare_v2(dbConnection->getConnectionPtr(), 
                          query.c_str(),
                          -1, 
                          &stmt, 
                          nullptr);
    if(result != SQLITE_OK) {
      lastError = string(zErrMsg);
      sqlite3_free(zErrMsg);
    }

    for(int i=1; i<=args.size(); i++) {
        result = sqlite3_bind_text(stmt, i, (args[i-1]).c_str(), -1, nullptr);
        if(result != SQLITE_OK) {
            lastError = sqlite3_errmsg(dbConnection->getConnectionPtr());
            sqlite3_finalize(stmt);
            return false;
        }
    }

    this->stmt = stmt;
    return true;
}

sqlite3_stmt *SQLiteSelectOperation::getStatement() const
{
    return stmt;
}

