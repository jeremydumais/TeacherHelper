#pragma once

#include "assessment.h"
#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
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

class ASSESSMENTSTORAGE_API AssessmentStorage : public ManagementItemStorageBase<Assessment>
{
public:
    explicit AssessmentStorage(const DatabaseConnection &connection, 
                               const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);

    std::string getSelectCommand() const override;
    Assessment getItemFromRecord(const IStorageSelectOperation &record) const override;
    void postGetStep(std::list<Assessment> &items) override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const Assessment &item) const override;
    bool postInsertStep(const Assessment &item) override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const Assessment &item) const override;
    bool preUpdateStep(const Assessment &item) override;
    bool postUpdateStep(const Assessment &item) override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    QueryResult preDeleteStep(size_t id) override;
    bool isReferentialIntegrityConstraint(size_t id) override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
    
    std::list<Assessment> getAllItems() override;
    std::list<Assessment> getItemsByClassId(const size_t classId);
    boost::optional<Assessment> getItemById(const size_t id);
    std::list<Assessment> loadItemsFromDB(const std::string &whereClause = "");



    bool insertResults(size_t assessmentId, const std::vector<AssessmentResult> &resultsToAdd);
    bool updateResult(const AssessmentResult &resultToUpdate);
    bool removeResults(const std::vector<size_t> &assessmentResultIdsToRemove);
    std::map<size_t, std::vector<AssessmentResult>> loadAllResults(const std::string &whereClause = "");
private:
    std::map<size_t, std::vector<AssessmentResult>> oldResults;
};

