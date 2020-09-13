#include "subjectStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

TEST(SubjectStorage_getAllItems, NoError_ReturnListSubjects)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(SubjectStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(SubjectStorage_insertItem, ValidInsertWithNoExistingSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 0),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(Subject("History")));
}

TEST(SubjectStorage_insertItem, ValidInsertWithTwoExistingSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(Subject("History")));
}

TEST(SubjectStorage_insertItem, ValidInsertWithANewDefaultSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(Subject("History", true)));
}

TEST(SubjectStorage_insertItem, FailedInsertAtUpdateAllRowsToRemoveDefaultSelect_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error during selecting the default subject", 0),
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(Subject("History", true)));
    ASSERT_EQ("Error during selecting the default subject", storage.getLastError());
}

TEST(SubjectStorage_insertItem, FailedInsertAtUpdateAllRowsToRemoveDefault_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update of all rows to remove default")
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(Subject("History", true)));
    ASSERT_EQ("Error during the update of all rows to remove default", storage.getLastError());
}

TEST(SubjectStorage_insertItem, FailedInsertAtInsertSubject_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(true, ""),
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert subject")
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(Subject("History", true)));
    ASSERT_EQ("Error during the insert subject", storage.getLastError());
}

TEST(SubjectStorage_updateItem, ValidUpdateWithOneExistingSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(Subject("History")));
}

TEST(SubjectStorage_updateItem, ValidUpdateWithTwoExistingSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(Subject("History")));
}

TEST(SubjectStorage_updateItem, ValidUpdateWithANewDefaultSubject_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(Subject("History", true)));
}

TEST(SubjectStorage_updateItem, FailedUpdateAtUpdateAllRowsToRemoveDefault_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update of all rows to remove default")
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(Subject("History", true)));
    ASSERT_EQ("Error during the update of all rows to remove default", storage.getLastError());
}

TEST(SubjectStorage_updateItem, FailedUpdateAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update of subject")
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(Subject("History", true)));
    ASSERT_EQ("Error during the update of subject", storage.getLastError());
}

TEST(SubjectStorage_deleteItem, ValidDelete_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(SubjectStorage_deleteItem, FailedDelete_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(false, "Constraint error during the delete operation", QueryResult::CONSTRAINTERROR)
    }) };
    SubjectStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
    ASSERT_EQ("Constraint error during the delete operation", storage.getLastError());
}