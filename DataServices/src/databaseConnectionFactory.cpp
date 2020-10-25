#include "databaseConnectionFactory.h"
#include "databaseConnection.h"

using namespace std;

shared_ptr<IDatabaseConnection> DatabaseConnectionFactory::createNewConnection(const std::string &databaseName) 
{
    return make_shared<DatabaseConnection>(databaseName);
}
