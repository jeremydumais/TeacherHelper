#include "sqliteOperationFactory.h"
#include "sqliteDDLOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"

using namespace std;

std::unique_ptr<IStorageSelectOperation> SQLiteOperationFactory::createSelectOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteSelectOperation>(connection, query, args);
}

std::unique_ptr<IStorageInsertOperation> SQLiteOperationFactory::createInsertOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteInsertOperation>(connection, query, args);
}

std::unique_ptr<IStorageUpdateOperation> SQLiteOperationFactory::createUpdateOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteUpdateOperation>(connection, query, args);
}

std::unique_ptr<IStorageDeleteOperation> SQLiteOperationFactory::createDeleteOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteDeleteOperation>(connection, query, args);
}

std::unique_ptr<IStorageDDLOperation> SQLiteOperationFactory::createDDLOperation(const IDatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteDDLOperation>(connection, query, args);
}
