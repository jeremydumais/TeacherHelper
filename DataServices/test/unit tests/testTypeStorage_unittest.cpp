#include "testTypeStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDatabaseConnection.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>

using namespace std;

struct FakeTestTypeRow
{
    int id;
    string name;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, name }; 
    }
};

TEST(TestTypeStorage_getAllItems, NoError_ReturnListTestTypes)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { 
            FakeTestTypeRow { 1, "Practice" }, 
            FakeTestTypeRow { 2, "Exercice" } 
        })
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(TestTypeStorage_getAllItems, Error_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(TestTypeStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_TRUE(storage.insertItem(TestType("Exam")));
}

TEST(TestTypeStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_FALSE(storage.insertItem(TestType("Exam")));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(TestTypeStorage_updateItem, ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_TRUE(storage.updateItem(TestType("Exam")));
}

TEST(TestTypeStorage_updateItem, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_FALSE(storage.updateItem(TestType("Exam")));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(TestTypeStorage_deleteItem, ValidDelete_ReturnOK)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(TestTypeStorage_deleteItem, FailedDelete_ReturnError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error during the delete operation", QueryResult::ERROR)
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(1));
    ASSERT_EQ("Error during the delete operation", storage.getLastError());
}

TEST(TestTypeStorage_deleteItem, FailedDeleteByReferentialIntegrityContrainst_ReturnConstraintError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 2 } })
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
}

TEST(TestTypeStorage_isReferentialIntegrityConstraint, FailedAtRetreiveReferentialIntegrity_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error during fetching referential integrity constraint")
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_FALSE(storage.isReferentialIntegrityConstraint(1));
    ASSERT_EQ("Error during fetching referential integrity constraint", storage.getLastError());
}

TEST(TestTypeStorage_isReferentialIntegrityConstraint, FailedAtRetreiveReferentialIntegrityRow_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { })
    }) };
    TestTypeStorage storage(FakeDatabaseConnection(), move(factory));

    ASSERT_FALSE(storage.isReferentialIntegrityConstraint(1));
    ASSERT_EQ("Unable to retreive the referential integrity constraints.", storage.getLastError());
}
