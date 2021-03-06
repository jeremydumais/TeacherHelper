#include "studentStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

struct FakeStudentRow
{
    int id;
    string firstname;
    string lastname;
    string comments;

    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, firstname, lastname, comments }; 
    }
};

TEST(StudentStorage_getAllItems, NoError_ReturnListStudents)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {
            FakeStudentRow {1, "Joe", "Blow", ""},
            FakeStudentRow {2, "Jane", "Doe", "A comment"}
        })
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(StudentStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(StudentStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(Student("Joe", "Blow")));
}

TEST(StudentStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(Student("Joe", "Blow")));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(StudentStorage_updateItem, ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(Student("Joe", "Blow")));
}

TEST(StudentStorage_updateItem, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(Student("Joe", "Blow")));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(StudentStorage_deleteItem, ValidDelete_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(StudentStorage_deleteItem, FailedDelete_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(false, "Constraint error during the delete operation", QueryResult::CONSTRAINTERROR)
    }) };
    StudentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
    ASSERT_EQ("Constraint error during the delete operation", storage.getLastError());
}