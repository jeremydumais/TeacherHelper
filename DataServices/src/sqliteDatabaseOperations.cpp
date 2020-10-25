#include "sqliteDatabaseOperations.h"

using namespace std;

SQLiteDatabaseOperations::SQLiteDatabaseOperations() 
    : db(nullptr)
{
}

int SQLiteDatabaseOperations::open(const std::string &fileName) 
{
    return sqlite3_open(fileName.c_str(), &db);
}

int SQLiteDatabaseOperations::close() 
{
    int retVal = sqlite3_close(db);
    db = nullptr;
    return retVal;
}

void* SQLiteDatabaseOperations::getConnectionPtr() const
{
    return db;
}
