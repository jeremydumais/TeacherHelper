#pragma once

#include "testType.h"
#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
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

class TESTTYPESTORAGE_API TestTypeStorage : public ManagementItemStorageBase<TestType>
{
public:
    explicit TestTypeStorage(const IDatabaseConnection &connection, 
                         const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);

    std::string getSelectCommand() const override;
    TestType getItemFromRecord(const IStorageSelectOperation &record) const override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const TestType &item) const override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const TestType &item) const override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
};