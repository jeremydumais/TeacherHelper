#include "databaseController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeDatabaseConnection : public IDatabaseConnection
{
public:
    FakeDatabaseConnection()
		: dbName("dbtest"s),
		  isDBOpened(false) {}
	const std::string &getDbName() const override { return dbName; };
    void open() override {};
	bool isOpened() const override { return isDBOpened; }
    void close() override {};
	string dbName;
	bool isDBOpened;
};

TEST(DatabaseController_Constructor, WithValidDatabaseConnection_ReturnSuccess)
{
	DatabaseController controller(unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>()));
	ASSERT_EQ("dbtest"s, controller.getOpenedDatabaseName());
}

TEST(DatabaseController_Constructor, WithNoDatabaseConnection_ReturnSuccess)
{
	DatabaseController controller;
	ASSERT_EQ(""s, controller.getOpenedDatabaseName());
}

TEST(DatabaseController_getDatabaseName, WithMyTestOnOpenedConnection_ReturnMyTest)
{
	auto storage { make_unique<FakeDatabaseConnection>() };
	storage->dbName = "myTest";
	DatabaseController controller(move(storage));

	ASSERT_EQ("myTest"s, controller.getOpenedDatabaseName());
}

TEST(DatabaseController_isDatabaseOpened, WithNoConnection_ReturnFalse)
{
	DatabaseController controller;
	ASSERT_FALSE(controller.isDatabaseOpened());
}

TEST(DatabaseController_isDatabaseOpened, WithConnectionClose_ReturnFalse)
{
	auto storage { make_unique<FakeDatabaseConnection>() };
	storage->isDBOpened = false;
	DatabaseController controller { move(storage) };
	ASSERT_FALSE(controller.isDatabaseOpened());
}

TEST(DatabaseController_isDatabaseOpened, WithConnectionOpened_ReturnTrue)
{
	auto storage { make_unique<FakeDatabaseConnection>() };
	storage->isDBOpened = true;
	DatabaseController controller { move(storage) };
	ASSERT_TRUE(controller.isDatabaseOpened());
}

TEST(DatabaseController_getDatabaseName, WithMyTestNoOpenedConnection_ReturnMyTest)
{
	DatabaseController controller;
	ASSERT_EQ(""s, controller.getOpenedDatabaseName());
}

TEST(DatabaseController_openDatabase, WithValidConnection_ReturnSuccess)
{
	DatabaseController controller(unique_ptr<IDatabaseConnection>(make_unique<FakeDatabaseConnection>()));
	controller.openDatabase("test"s);
}

TEST(DatabaseController_closeDatabase, WithValidOpenedConnection_ReturnSuccess)
{
	auto storage { make_unique<FakeDatabaseConnection>() };
	storage->isDBOpened = true;
	DatabaseController controller(move(storage));
	controller.closeDatabase();
}

TEST(DatabaseController_closeDatabase, WithNoConnection_ThrowRuntimeError)
{
	try {
		DatabaseController controller;
		controller.closeDatabase();
	}
	catch(runtime_error &err) {
		ASSERT_STREQ("No connection has been supplied.", err.what());
	}
}

TEST(DatabaseController_closeDatabase, WithAlreadyClosedConnection_ThrowRuntimeError)
{
	try {
		auto storage { make_unique<FakeDatabaseConnection>() };
		storage->isDBOpened = false;
		DatabaseController controller(move(storage));
		controller.closeDatabase();
	}
	catch(runtime_error &err) {
		ASSERT_STREQ("Cannot close an already closed database.", err.what());
	}
}