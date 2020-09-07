#pragma once

#include "testType.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "IStorageOperationFactory.h"
#include <list>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define TESTTYPESTORAGE_API __declspec(dllexport)   
    #else  
        #define TESTTYPESTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define TESTTYPESTORAGE_API
#endif

class TESTTYPESTORAGE_API TestTypeStorage : public IManagementItemStorage<TestType>
{
public:
    explicit TestTypeStorage(const DatabaseConnection &connection, 
                         const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::list<TestType> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const TestType &testType) override;
    bool updateItem(const TestType &testType) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};