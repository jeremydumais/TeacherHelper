#pragma once

#include "subject.h"
#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SUBJECTSTORAGE_API __declspec(dllexport)   
    #else  
        #define SUBJECTSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define SUBJECTSTORAGE_API
#endif

class SUBJECTSTORAGE_API SubjectStorage : public ManagementItemStorageBase<Subject>
{
public:
    explicit SubjectStorage(const DatabaseConnection &connection, 
                            const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::string getSelectCommand() const override;
    Subject getItemFromRecord(const IStorageSelectOperation &record) const override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const Subject &item) const override;
    bool preInsertStep(const Subject &item) override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const Subject &item) const override;
    bool preUpdateStep(const Subject &item) override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
    bool updateAllRowsToRemoveDefault(size_t currentSubjectId);
};