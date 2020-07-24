#pragma once

#include "testType.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class TestTypeController
{
public:
    explicit TestTypeController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<TestType>> managementItemStorage = nullptr);
    const std::list<TestType> &getTestTypes() const;
    const TestType* findTestType(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
						 size_t currentlyEditedTestTypeId) const;
    const std::string &getLastError() const;
    void loadTestTypes();
    bool insertTestType(const TestType &testType);
    bool updateTestType(const TestType &testType);
    bool deleteTestType(size_t id);
private:
	std::list<TestType> testTypes;
    std::unique_ptr<IManagementItemStorage<TestType>> storage;
    std::string lastError;
};