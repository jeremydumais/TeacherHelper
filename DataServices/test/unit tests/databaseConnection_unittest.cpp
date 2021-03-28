#include "databaseConnection.h"
#include "FakeDatabaseOperations.h"
#include "FakeFileSystemOperations.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResult.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

TEST(DatabaseConnection_Constructor, EmptyDbName_ThrowInvalidArgument)
{
	try
	{
		DatabaseConnection dbConn("", 
								  move(make_unique<FakeFileSystemOperations>()),
								  move(make_unique<FakeDatabaseOperations>()),
								  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("dbName cannot be null or empty.", err.what());
	}
}

TEST(DatabaseConnection_Constructor, WhiteSpaceDbName_ThrowInvalidArgument)
{
	try
	{
		DatabaseConnection dbConn("    ", 
								  move(make_unique<FakeFileSystemOperations>()),
								  move(make_unique<FakeDatabaseOperations>()),
								  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("dbName cannot be null or empty.", err.what());
	}
}

TEST(DatabaseConnection_Constructor, ValidDbName_ReturnValid)
{
	DatabaseConnection dbConn("teacherdb", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(make_unique<FakeDatabaseOperations>()),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
}

TEST(DatabaseConnection_getDbName, TestDbName_ReturnValid)
{
	DatabaseConnection dbConn("Test", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(make_unique<FakeDatabaseOperations>()),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
	ASSERT_EQ("Test", dbConn.getDbName());
}

TEST(DatabaseConnection_getDbName, TestMoreDbName_ReturnValid)
{
	DatabaseConnection dbConn("TestMore", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(make_unique<FakeDatabaseOperations>()),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
	ASSERT_EQ("TestMore", dbConn.getDbName());
}

TEST(DatabaseConnection_getConnectionPtr, TestWithNullPtr_ReturnNullPtr)
{
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->getConnectionPtrReturnValue = nullptr;
	DatabaseConnection dbConn("TestMore", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(fakeDatabaseOperations),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
	ASSERT_EQ(nullptr, dbConn.getConnectionPtr());
}

TEST(DatabaseConnection_getConnectionPtr, TestWithAssignedPtr_ReturnNullPtr)
{
	auto i = make_unique<int>(5);
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->getConnectionPtrReturnValue = i.get();
	DatabaseConnection dbConn("TestMore", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(fakeDatabaseOperations),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
	ASSERT_EQ(i.get(), dbConn.getConnectionPtr());
}

TEST(DatabaseConnection_open, NotAValidFileDbName_ThrowRuntimeError)
{
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = false;
	DatabaseConnection dbConn("notAValidFile", move(fakeFileSystem));
	try
	{
		dbConn.open();
		FAIL();
	}
	catch(runtime_error &err) 
	{
        ASSERT_STREQ("The database notAValidFile does not exist.", err.what());
	}
}

TEST(DatabaseConnection_open, ErrorAtOpeningDB_ThrowRuntimeError)
{
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = true;
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->openReturnValue = 1;
	DatabaseConnection dbConn("validFile", 
								move(fakeFileSystem),
								move(fakeDatabaseOperations));
	try
	{
		dbConn.open();
		FAIL();
	}
	catch(runtime_error &err) 
	{
        ASSERT_STREQ("Cannot open database validFile. sqlite3_errmsg(db)", err.what());
	}
}

TEST(DatabaseConnection_open, ErrorAtDDLQuery_ThrowRuntimeError)
{
	auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
		FakeOperationResultFactory::createNewDDLResult(false, "Error in DDL query")
	}) };
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = true;
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->openReturnValue = 0;
	DatabaseConnection dbConn("validFile", 
								move(fakeFileSystem),
								move(fakeDatabaseOperations),
								move(factory));
	try
	{
		dbConn.open();
		FAIL();
	}
	catch(runtime_error &err) 
	{
        ASSERT_STREQ("Error in DDL query", err.what());
	}
}

TEST(DatabaseConnection_open, AllStepsValid_ReturnSuccess)
{
	auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
		FakeOperationResultFactory::createNewDDLResult(true, "")
	}) };
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = true;
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->openReturnValue = 0;
	DatabaseConnection dbConn("validFile", 
								move(fakeFileSystem),
								move(fakeDatabaseOperations),
								move(factory));

	dbConn.open();
}

TEST(DatabaseConnection_isOpened, DatabaseIsOpen_ReturnTrue)
{
	auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
		FakeOperationResultFactory::createNewDDLResult(true, "")
	}) };
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = true;
	auto fakeDatabaseOperations { make_unique<FakeDatabaseOperations>() };
	fakeDatabaseOperations->openReturnValue = 0;
	DatabaseConnection dbConn("validFile", 
								move(fakeFileSystem),
								move(fakeDatabaseOperations),
								move(factory));

	dbConn.open();
	ASSERT_TRUE(dbConn.isOpened());
}

TEST(DatabaseConnection_open, DatabaseIsNotOpen_ReturnFalse)
{
	auto fakeFileSystem { make_unique<FakeFileSystemOperations>() };
	fakeFileSystem->isFileExists = false;
	DatabaseConnection dbConn("notAValidFile", move(fakeFileSystem));
	try
	{
		dbConn.open();
	}
	catch(runtime_error &err) 
	{
        ASSERT_STREQ("The database notAValidFile does not exist.", err.what());
	}
	ASSERT_FALSE(dbConn.isOpened());
}

TEST(DatabaseConnection_close, closeValidConnection_ReturnSuccess)
{
	DatabaseConnection dbConn("Test", 
							  move(make_unique<FakeFileSystemOperations>()),
							  move(make_unique<FakeDatabaseOperations>()),
							  move(make_unique<FakeOperationFactory>(vector<FakeOperationResult> {})));
	dbConn.close();
	ASSERT_FALSE(dbConn.isOpened());
}