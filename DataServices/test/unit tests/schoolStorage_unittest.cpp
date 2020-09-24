#include "schoolStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>

using namespace std;

struct FakeSchoolRow
{
    int id;
    string name;
    int cityId;
    string cityName;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, name, cityId, cityName }; 
    }
};

TEST(SchoolStorage_getAllItems, NoError_ReturnListSchools)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {
            FakeSchoolRow {1, "School1", 1, "New York"},
            FakeSchoolRow {2, "School2", 2, "Los Angeles"}
        })
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(SchoolStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(SchoolStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(School("First grade", City("New York"))));
}

TEST(SchoolStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(School("First grade", City("New York"))));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(SchoolStorage_updateItem, ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(School("First grade", City("New York"))));
}

TEST(SchoolStorage_updateItem, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(School("First grade", City("New York"))));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(SchoolStorage_deleteItem, ValidDelete_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(SchoolStorage_deleteItem, FailedDelete_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(false, "Constraint error during the delete operation", QueryResult::CONSTRAINTERROR)
    }) };
    SchoolStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
    ASSERT_EQ("Constraint error during the delete operation", storage.getLastError());
}