#include "databaseController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeDatabaseConnection : public IDatabaseConnection
{
public:
    FakeDatabaseConnection()
		: dbName("dbtest"s) {}
	const std::string &getDbName() const override { return dbName; };
    void open() override {};
    void close() override {};
	string dbName;
};

TEST(DatabaseController_Constructor, ValidArguments_ReturnSuccess)
{
	DatabaseController controller("test"s, unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>()));
}

TEST(DatabaseController_openDatabase, ReturnSuccess)
{
	DatabaseController controller("test"s, unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>()));
	controller.openDatabase();
}

TEST(DatabaseController_closeDatabase, ReturnSuccess)
{
	DatabaseController controller("test"s, unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>()));
	controller.closeDatabase();
}

TEST(DatabaseController_getDatabaseName, WithTest_ReturnTest)
{
	auto storage { unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>())};
	DatabaseController controller("test"s, move(storage));
	ASSERT_EQ("dbtest"s, controller.getDatabaseName());
}