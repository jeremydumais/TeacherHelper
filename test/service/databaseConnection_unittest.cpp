#include "../../src/service/databaseConnection.h"
#include <gtest/gtest.h>

using namespace std;

TEST(DatabaseConnection_Constructor, NullDbName)
{
	try
	{
		DatabaseConnection dbConn(nullptr);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("dbName cannot be null or empty", err.what());
	}
}