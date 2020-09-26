#include "sqliteOperationFactory.h"
#include "sqliteDeleteOperation.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"

using namespace std;

std::unique_ptr<IStorageSelectOperation> SQLiteOperationFactory::createSelectOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteSelectOperation>(connection, query, args);
}

std::unique_ptr<IStorageInsertOperation> SQLiteOperationFactory::createInsertOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteInsertOperation>(connection, query, args);
}

std::unique_ptr<IStorageUpdateOperation> SQLiteOperationFactory::createUpdateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteUpdateOperation>(connection, query, args);
}

std::unique_ptr<IStorageDeleteOperation> SQLiteOperationFactory::createDeleteOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteDeleteOperation>(connection, query, args);
}
