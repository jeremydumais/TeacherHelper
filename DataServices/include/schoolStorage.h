#pragma once

#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
#include "school.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SCHOOLSTORAGE_API __declspec(dllexport)   
    #else  
        #define SCHOOLSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define SCHOOLSTORAGE_API
#endif

class SCHOOLSTORAGE_API SchoolStorage : public ManagementItemStorageBase<School>
{
public:
    explicit SchoolStorage(const IDatabaseConnection &connection, 
                           const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);

    std::string getSelectCommand() const override;
    School getItemFromRecord(const IStorageSelectOperation &record) const override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const School &item) const override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const School &item) const override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
};