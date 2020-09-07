#include "testTypeStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

TEST(TestTypeStorage_getAllItems, NoError_ReturnListTestTypes)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2)
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(TestTypeStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(TestTypeStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(TestType("Exam")));
}

TEST(TestTypeStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(TestType("Exam")));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(TestTypeStorage_updateItem, ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(TestType("Exam")));
}

TEST(TestTypeStorage_updateItem, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(TestType("Exam")));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(TestTypeStorage_deleteItem, ValidDelete_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(TestTypeStorage_deleteItem, FailedDelete_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(false, "Constraint error during the delete operation", QueryResult::CONSTRAINTERROR)
    }) };
    TestTypeStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
    ASSERT_EQ("Constraint error during the delete operation", storage.getLastError());
}