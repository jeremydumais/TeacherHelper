#pragma once

#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
#include "student.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define STUDENTSTORAGE_API __declspec(dllexport)   
    #else  
        #define STUDENTSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define STUDENTSTORAGE_API
#endif

class STUDENTSTORAGE_API StudentStorage : public ManagementItemStorageBase<Student>
{
public:
    explicit StudentStorage(const DatabaseConnection &connection, 
                            const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::string getSelectCommand() const override;
    Student getItemFromRecord(const IStorageSelectOperation &record) const override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const Student &item) const override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const Student &item) const override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
};