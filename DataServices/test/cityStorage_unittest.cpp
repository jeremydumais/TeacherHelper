#include "cityStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include <gtest/gtest.h>

using namespace std;

class FakeUpdateOperation : public IStorageUpdateOperation
{
public:
    FakeUpdateOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult)
        : IStorageUpdateOperation(connection, query, args),
          executeResult(executeResult) {}
    bool execute() override { return executeResult; }
    bool executeResult = true;
};

class FakeOperationFactory : public IStorageOperationFactory
{
public:
    std::unique_ptr<IStorageUpdateOperation> createUpateOperation(const DatabaseConnection &connection, 
        const std::string &query,
        const std::vector<std::string> &args) 
    {
        return make_unique<FakeUpdateOperation>(connection, query, args, executeResult);
    }
    bool executeResult = true;
};

TEST(CityStorage_UpdateOperation, ValidUpdate_ReturnTrue)
{
    CityStorage storage(DatabaseConnection("fake"), make_unique<FakeOperationFactory>());

    ASSERT_TRUE(storage.updateItem(City("New York")));
}

TEST(CityStorage_UpdateOperation, FailedUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>() };
    factory->executeResult = false;
    CityStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(City("New York")));
}