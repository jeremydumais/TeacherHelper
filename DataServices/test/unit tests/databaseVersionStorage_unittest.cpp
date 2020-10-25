#include "databaseVersionStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDatabaseConnection.h"
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

TEST(DatabaseVersionStorage_getVersion, WhenTableDoesntExist_ReturnVersion100)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {}),
        FakeOperationResultFactory::createNewSelectResult(true, "", { FakeVersionRow { "1.0.0" }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(Version(1, 0, 0), actual);
}

TEST(DatabaseVersionStorage_getVersion, ErrorAtSelectingTableExist_ReturnNoVersion)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error at looking for the version tables")
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_FALSE(actual.has_value());
    ASSERT_EQ("Error at looking for the version tables"s, storage.getLastError());
}

TEST(DatabaseVersionStorage_getVersion, ErrorAtSelectingRowTableExist_ReturnVersion100)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(Version(1, 0, 0), actual);
}

TEST(DatabaseVersionStorage_getVersion, ErrorAtSelectingTableExistRowReturnedButNotVersion_ReturnVersion100)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { { "test"s }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(Version(1, 0, 0), actual);
}

TEST(DatabaseVersionStorage_getVersion, WithVersion100_ReturnVersion100)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { FakeVersionRow { "1.0.0" }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(Version(1, 0, 0), actual);
}

TEST(DatabaseVersionStorage_getVersion, WithVersion234_ReturnVersion234)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { FakeVersionRow { "2.3.4" }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_TRUE(actual.has_value());
    ASSERT_EQ(Version(2, 3, 4), actual);
}

TEST(DatabaseVersionStorage_getVersion, ErrorAtSelectingVersionTable_ReturnNoVersion)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(false, "Error at selection version table."s)
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_FALSE(actual.has_value());
    ASSERT_EQ("Error at selection version table."s, storage.getLastError());
}

TEST(DatabaseVersionStorage_getVersion, ErrorAtSelectingVersionTableRow_ReturnNoVersion)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, ""s, { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(true, "Error at selecting row"s, {})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_FALSE(actual.has_value());
    ASSERT_EQ("Error at selecting row"s, storage.getLastError());
}

TEST(DatabaseVersionStorage_getVersion, ErrorVersionTableRowEmptyString_ReturnNoVersion)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, ""s, { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(true, ""s, { FakeVersionRow { "" }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_FALSE(actual.has_value());
}

TEST(DatabaseVersionStorage_getVersion, ErrorVersionTableRowOutOfRangeValues_ReturnNoVersion)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, ""s, { { "version"s } }),
        FakeOperationResultFactory::createNewSelectResult(true, ""s, { FakeVersionRow { "666.666.666" }})
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    auto actual = storage.getVersion();
    ASSERT_FALSE(actual.has_value());
}

TEST(DatabaseVersionStorage_updateVersion, Version101_ReturnTrue)
{
        auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true, ""s)
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    ASSERT_TRUE(storage.updateVersion(Version(1, 0, 1)));
}

TEST(DatabaseVersionStorage_updateVersion, ErrorAtUpdating_ReturnFalse)
{
        auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "Error while updating version"s)
    }) };

    DatabaseVersionStorage storage(FakeDatabaseConnection(), move(factory));
    ASSERT_FALSE(storage.updateVersion(Version(1, 0, 1)));
    ASSERT_EQ("Error while updating version"s, storage.getLastError());
}