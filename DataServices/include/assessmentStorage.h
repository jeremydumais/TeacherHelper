#pragma once

#include "assessment.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "IStorageOperationFactory.h"
#include <list>
#include <map>
#include <vector>
#include <boost/optional.hpp>

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
    explicit AssessmentStorage(const DatabaseConnection &connection, 
                               const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::list<Assessment> getAllItems() override;
    std::list<Assessment> getItemsByClassId(const size_t classId);
    boost::optional<Assessment> getItemById(const size_t id);
    std::list<Assessment> loadItemsFromDB(const std::string &whereClause = "");
    const std::string &getLastError() const override;
    bool insertItem(const Assessment &assessment) override;
    size_t retreiveAssignedAssessmentId();
    bool insertResults(size_t assessmentId, const std::vector<AssessmentResult> &resultsToAdd);
    bool updateItem(const Assessment &assessment) override;
    bool updateResult(const AssessmentResult &resultToUpdate);
    bool removeResults(const std::vector<size_t> &assessmentResultIdsToRemove);
    QueryResult deleteItem(size_t id) override;
    std::map<size_t, std::vector<AssessmentResult>> loadAllResults(const std::string &whereClause = "");

private:
    const DatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};

