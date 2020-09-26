#include "cityStorage.h"
#include "databaseConnection.h"
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace boost;

class CityStorageTest : public ::testing::Test
{
public:
    CityStorageTest()
    {
        //Create the test database
        connection = make_unique<DatabaseConnection>(dbName);
        connection->create();
        storage = make_unique<CityStorage>(*connection.get());
    }

    ~CityStorageTest()
    {
        //Remove the test database
        connection->close();
        filesystem::remove(dbName);

    }
    string dbName = "integration_tests_db";
    unique_ptr<DatabaseConnection> connection;
	unique_ptr<CityStorage> storage;
};

TEST_F(CityStorageTest, OpenExistingDB_ReturnSuccess)
{
    storage.release();
    connection->close();
    connection->open();
    storage = make_unique<CityStorage>(*connection.get());

    auto cities = storage->getAllItems();
    ASSERT_EQ(0, cities.size());
}

TEST_F(CityStorageTest, OpenNonExistingDB_ReturnSuccess)
{
    storage.release();
    connection.release();
    filesystem::remove(dbName);
    dbName = "nonExistantdb";
    connection = make_unique<DatabaseConnection>(dbName);
    try {
        connection->open();
        FAIL();
    }
    catch(runtime_error &err) {
    }
}

TEST_F(CityStorageTest, InsertCity_ReturnSuccess)
{
    storage->insertItem(City("New York"));
    auto cities = storage->getAllItems();
    ASSERT_EQ(1, cities.size());
    ASSERT_EQ("New York", cities.begin()->getName());
}

TEST_F(CityStorageTest, UpdateCity_ReturnSuccess)
{
    storage->insertItem(City("Chicago"));
    storage->insertItem(City("New York"));
    auto cities = storage->getAllItems();
    ASSERT_EQ(2, cities.size());

    auto iter = cities.begin();
    iter->setName("Los Angeles");
    storage->updateItem(*iter);

    cities = storage->getAllItems();
    iter = cities.begin();
    ASSERT_EQ(2, cities.size());

    ASSERT_EQ(1, iter->getId());
    ASSERT_EQ("Los Angeles", iter->getName());
    iter++;
    ASSERT_EQ(2, iter->getId());
    ASSERT_EQ("New York", iter->getName());
}

TEST_F(CityStorageTest, DeleteCity_ReturnSuccess)
{
    storage->insertItem(City("Chicago"));
    storage->insertItem(City("New York"));
    auto cities = storage->getAllItems();
    ASSERT_EQ(2, cities.size());

    auto iter = cities.begin();
    storage->deleteItem(iter->getId());

    cities = storage->getAllItems();
    iter = cities.begin();
    ASSERT_EQ(1, cities.size());

    ASSERT_EQ(2, iter->getId());
    ASSERT_EQ("New York", iter->getName());
}