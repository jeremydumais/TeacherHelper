#pragma once

#include "testType.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include <list>

class TestTypeStorage : public IManagementItemStorage<TestType>
{
public:
    explicit TestTypeStorage(const DatabaseConnection &connection);
    std::list<TestType> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const TestType &testType) override;
    bool updateItem(const TestType &testType) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};