#pragma once

#include "assessment.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "sqliteDeleteOperation.h"
#include <list>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define ASSESSMENTSTORAGE_API __declspec(dllexport)   
    #else  
        #define ASSESSMENTSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define ASSESSMENTSTORAGE_API
#endif

class ASSESSMENTSTORAGE_API AssessmentStorage : public IManagementItemStorage<Assessment>
{
public:
    explicit AssessmentStorage(const DatabaseConnection &connection);
    std::list<Assessment> getAllItems() override;
    std::list<Assessment> getItemsByClassId(const size_t classId);
    std::list<Assessment> loadItemsFromDB(const std::string &whereClause = "");
    const std::string &getLastError() const override;
    bool insertItem(const Assessment &assessment) override;
    size_t retreiveAssignedAssessmentId();
    bool updateItem(const Assessment &assessment) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};

