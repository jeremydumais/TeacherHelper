#pragma once

#include "testType.h"
#include "ManagementItemStorageBase.h"
#include "IDatabaseController.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define TESTTYPECONTROLLER_API __declspec(dllexport)   
    #else  
        #define TESTTYPECONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define TESTTYPECONTROLLER_API
#endif

class TESTTYPECONTROLLER_API TestTypeController
{
public:
    explicit TestTypeController(const IDatabaseController &databaseController,
                            std::unique_ptr<ManagementItemStorageBase<TestType>> managementItemStorage = nullptr);
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
    std::unique_ptr<ManagementItemStorageBase<TestType>> storage;
    std::string lastError;
};