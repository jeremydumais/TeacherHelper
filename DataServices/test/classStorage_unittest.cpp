#include "classStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

TEST(ClassStorage_getAllItems, TwoClassesWithNoMembersNoError_ReturnListClasses)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(true, "", 0)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(ClassStorage_getAllItems, TwoClassesWithTwoMembersNoError_ReturnListClasses)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(true, "", 2)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(ClassStorage_getAllItems, ErrorLoadingClass_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(ClassStorage_getAllItems, ErrorLoadingMembers_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.insertItem(myClass));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsertButFailedAtRetreivingRecordId_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(false, "Error while retreiving the id")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.insertItem(myClass));
    ASSERT_EQ("Error while retreiving the id", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsertAndAddingOneMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_insertItem, ValidInsertAndAddingTwoMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));
    myClass.addMember(Student(3, "Jane", "Dow"));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_updateItem, ValidUpdateWithNoMembers_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 0),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtLoadMembers_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error during the update operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(ClassStorage_updateItem, FailedUpdateAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewSelectResult(false, "Error during the update operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(ClassStorage_updateItem, ValidUpdateWithOneRemovedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, ValidUpdateWithTwoRemovedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtRemovingMember_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error at removing member")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error at removing member", storage.getLastError());
}

TEST(ClassStorage_updateItem, ValidUpdateWithOneAddedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtAddingMember_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewInsertResult(false, "Error at adding member")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error at adding member", storage.getLastError());
}
/*
TEST(ClassStorage_deleteItem, ValidDelete_ReturnTrue)
{
    ClassStorage storage(DatabaseConnection("fake"), make_unique<FakeOperationFactory>(vector<FakeOperationResult> { FakeOperationResult() }));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}*/

/*TEST(ClassStorage_deleteItem, FailedDelete_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>() };
    factory->executeResult = false;
    factory->extendedResultInfo = QueryResult::CONSTRAINTERROR;
    factory->lastError = "Constraint error during the delete operation";

    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
    ASSERT_EQ("Constraint error during the delete operation", storage.getLastError());
}*/