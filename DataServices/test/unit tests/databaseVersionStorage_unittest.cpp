#include "databaseVersionStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>

using namespace std;

struct FakeVersionRow
{
    string name;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { name }; 
    }
};

TEST(DatabaseVersionStorage_getVersion100, NoError_Return100)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { 
            FakeVersionRow { "1.0.0" }
        })
    }) };

    DatabaseVersionStorage storage(DatabaseConnection("fake"), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(1, actual->getMajor());
    ASSERT_EQ(0, actual->getMinor());
    ASSERT_EQ(0, actual->getPatch());
}
