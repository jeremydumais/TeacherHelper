#include "databaseManagementOperations.h"
#include "FakeDatabaseConnectionFactory.h"
#include "FakeDatabaseOperations.h"
#include "FakeFileSystemOperations.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

TEST(DatabaseManagementOperations_exist, WithExistantDatabase_ReturnTrue) 
{
    auto fsOperations = make_shared<FakeFileSystemOperations>();
    fsOperations->isFileExists = true;
    DatabaseManagementOperations operations { fsOperations };
    ASSERT_TRUE(operations.exist("ValidDbName"));
}

TEST(DatabaseManagementOperations_exist, WithNonExistantDatabase_ReturnFalse) 
{
    auto fsOperations = make_shared<FakeFileSystemOperations>();
    fsOperations->isFileExists = false;
    DatabaseManagementOperations operations { fsOperations };
    ASSERT_FALSE(operations.exist("ValidDbName"));
}

TEST(DatabaseManagementOperations_create, WithErrorAtOpening_ReturnFalse) 
{
    auto connectionFactory { make_unique<FakeDatabaseConnectionFactory>()};
    connectionFactory->isOpenForCreationThrowRuntimeError = true;
    DatabaseManagementOperations operations { make_shared<FakeFileSystemOperations>(),
                                              make_shared<FakeDatabaseOperations>(),
                                              move(connectionFactory) };
    ASSERT_FALSE(operations.create("ValidDbName"));
    ASSERT_EQ("Cannot create database ValidDbName. sqlite3_errmsg(db)"s, operations.getLastError());
}

TEST(DatabaseManagementOperations_create, WithErrorAtCreatingTables_ReturnFalse) 
{
    auto factory { make_shared<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDDLResult(false, "Error when creating tables")
    }) };
    DatabaseManagementOperations operations { make_shared<FakeFileSystemOperations>(),
                                              make_shared<FakeDatabaseOperations>(),
                                              move(make_unique<FakeDatabaseConnectionFactory>()),
                                              factory };
    ASSERT_FALSE(operations.create("ValidDbName"));
    ASSERT_EQ("Error when creating tables"s, operations.getLastError());
}


TEST(DatabaseManagementOperations_create, WithValidFile_ReturnTrue) 
{
    auto factory { make_shared<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, ""),
        FakeOperationResultFactory::createNewDDLResult(true, "")
    }) };
    DatabaseManagementOperations operations { make_shared<FakeFileSystemOperations>(),
                                              make_shared<FakeDatabaseOperations>(),
                                              move(make_unique<FakeDatabaseConnectionFactory>()),
                                              factory };
    ASSERT_TRUE(operations.create("ValidDbName"));
}
