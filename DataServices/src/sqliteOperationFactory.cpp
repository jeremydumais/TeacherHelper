#include "sqliteOperationFactory.h"
#include "sqliteUpdateOperation.h"

using namespace std;

std::unique_ptr<IStorageUpdateOperation> SQLiteOperationFactory::createUpateOperation(const DatabaseConnection &connection, 
                                             const std::string &query,
                                             const std::vector<std::string> &args) 
{
    return make_unique<SQLiteUpdateOperation>(connection, query, args);
}
