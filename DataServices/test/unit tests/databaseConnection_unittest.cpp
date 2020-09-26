#include "databaseConnection.h"
#include <gtest/gtest.h>

using namespace std;

TEST(DatabaseConnection_Constructor, EmptyDbName_ThrowInvalidArgument)
{
	try
	{
		DatabaseConnection dbConn("");
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
		DatabaseConnection dbConn("    ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("dbName cannot be null or empty.", err.what());
	}
}

TEST(DatabaseConnection_Constructor, ValidDbName_ReturnValid)
{
	DatabaseConnection dbConn("teacherdb");
}

TEST(DatabaseConnection_getDbName, TestDbName_ReturnValid)
{
	DatabaseConnection dbConn("Test");
	ASSERT_EQ("Test", dbConn.getDbName());
}

TEST(DatabaseConnection_getDbName, TestMoreDbName_ReturnValid)
{
	DatabaseConnection dbConn("TestMore");
	ASSERT_EQ("TestMore", dbConn.getDbName());
}

TEST(DatabaseConnection_open, NotAValidFileDbName_ThrowRuntimeError)
{
	try
	{
		DatabaseConnection dbConn("notAValidFile");
		dbConn.open();
		FAIL();
	}
	catch(runtime_error &err) 
	{
        ASSERT_STREQ("The database notAValidFile does not exist.", err.what());
	}
}