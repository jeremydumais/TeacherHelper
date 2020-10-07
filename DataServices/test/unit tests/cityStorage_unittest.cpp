#include "cityStorage.h"
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

struct FakeCityRow
{
    int id;
    string name;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, name }; 
    }
};

TEST(CityStorage_getAllItems, NoError_ReturnListCities)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { 
            FakeCityRow { 1, "New York" }, 
            FakeCityRow { 2, "Los Angeles" } 
        })
    }) };

    CityStorage storage(DatabaseConnection("fake"), move(factory));
    auto actual = storage.getAllItems();
    ASSERT_EQ(2, actual.size());
}

TEST(CityStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(CityStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(City("New York")));
}

TEST(CityStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(City("New York")));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(CityStorage_updateItem, ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(City("New York")));
}

TEST(CityStorage_updateItem, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(City("New York")));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(CityStorage_deleteItem, ValidDelete_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(CityStorage_deleteItem, FailedDelete_ReturnError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error during the delete operation", QueryResult::ERROR)
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(1));
    ASSERT_EQ("Error during the delete operation", storage.getLastError());
}

TEST(CityStorage_deleteItem, FailedDeleteByReferentialIntegrityContrainst_ReturnConstraintError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 2 } })
    }) };
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
}

