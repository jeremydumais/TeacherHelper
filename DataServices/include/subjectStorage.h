#pragma once

#include "subject.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "IStorageOperationFactory.h"
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

class SUBJECTSTORAGE_API SubjectStorage : public IManagementItemStorage<Subject>
{
public:
    explicit SubjectStorage(const DatabaseConnection &connection, 
                            const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::list<Subject> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Subject &subject) override;
    bool updateItem(const Subject &subject) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
    bool updateAllRowsToRemoveDefault(size_t currentSubjectId);
};