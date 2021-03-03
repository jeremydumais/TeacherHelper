#include "assessmentStorage.h"
#include "sqliteDateTimeFactory.h"
#include "sqliteDeleteOperation.h"
#include "sqliteInsertOperation.h"
#include "sqliteOperationFactory.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

AssessmentStorage::AssessmentStorage(const IDatabaseConnection &connection, 
                                     unique_ptr<IStorageOperationFactory> operationFactory)
: ManagementItemStorageBase<Assessment>(connection, move(operationFactory)),
  oldResults(map<size_t, vector<AssessmentResult>>())
{
}

list<Assessment> AssessmentStorage::getAllItems()
{
    return loadItemsFromDB();
}

list<Assessment> AssessmentStorage::getItemsByClassId(const size_t classId) 
{
    return loadItemsFromDB(fmt::format("WHERE class.id = {0} ", classId));
}

boost::optional<Assessment> AssessmentStorage::getItemById(const size_t id) 
{
    auto items = loadItemsFromDB(fmt::format("WHERE assessment.id = {0} ", id));
    if (items.size() > 1) {
        lastError = "More than one assessment was returned.";
        return {};
    } 
    else if (items.size() < 1) {
        lastError = "No assessment was returned.";
        return {};
    }
    else {
        return *(items.begin());
    }
}

list<Assessment> AssessmentStorage::loadItemsFromDB(const string &whereClause) 
{
    list<Assessment> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        getSelectCommand() +
        whereClause +
        " ORDER BY datetime(date), assessment.name");
    if (operation->execute()) {
        while (operation->getRow()) {
            retVal.push_back(getItemFromRecord(*operation.get()));
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
        return list<Assessment>();
    }
    try {
        postGetStep(retVal);
    }
    catch(std::runtime_error &err) {
        lastError = err.what();
        return list<Assessment>();
    }
    return retVal;
}

std::string AssessmentStorage::getSelectCommand() const
{
    return "SELECT assessment.id, assessment.name, testType.id, testType.name, subject.id, subject.name, " 
           "class.id, class.name, school.id, school.name, city.id, city.name, date, maxScore "
           "FROM assessment "
           "INNER JOIN testType ON testType.id = assessment.testType_id " 
           "INNER JOIN subject ON subject.id = assessment.subject_id " 
           "INNER JOIN class ON class.id = assessment.class_id " 
           "INNER JOIN school ON school.id = class.school_id " 
           "INNER JOIN city ON city.id = school.city_id ";
}

Assessment AssessmentStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return Assessment(record.getIntValue(0),
                      record.getStringValue(1),
                      TestType(record.getIntValue(2),
                               record.getStringValue(3)),
                      Subject(record.getIntValue(4),
                              record.getStringValue(5)),
                      Class(record.getIntValue(6),
                            record.getStringValue(7),
                            School(record.getIntValue(8),
                                   record.getStringValue(9),
                                   City(record.getIntValue(10),
                                        record.getStringValue(11)))),
                      record.getDateTime(12).getBoostPTime(),
                      static_cast<float>(record.getDoubleValue(13)));
}

void AssessmentStorage::postGetStep(list<Assessment> &items) 
{
    //Load all results
    auto assessmentResults = loadAllResults();
    //Associate all members to classes
    for(auto &assessment : items) {
        auto it = assessmentResults.find(assessment.getId());
        if (it != assessmentResults.end()) {
            for(const auto &result : it->second) {
                assessment.addResult(result);
            }
            it++;
        }
    }
}

std::string AssessmentStorage::getInsertCommand() const
{
    return "INSERT INTO assessment (name, testType_id, subject_id, class_id, date, maxScore) "
           "VALUES(?, ?, ?, ?, ?, ?)";
}

std::vector<std::string> AssessmentStorage::getInsertValues(const Assessment &item) const
{
    auto assessmentDateTime = SQLiteDateTimeFactory::NewDateTimeFromPTime(item.getDateTime());
    return { boost::trim_copy(item.getName()),
             to_string(item.getTestType().getId()), 
             to_string(item.getSubject().getId()), 
             to_string(item.getClass().getId()), 
             assessmentDateTime.toSQLiteString(),
             to_string(item.getMaxScore())
           };
}

bool AssessmentStorage::postInsertStep(const Assessment &item) 
{
    //Retreive the assigned primary key (id)
    size_t assessmentId = retreiveAssignedRecordId();
    if (assessmentId == 0) {
        return false;
    }

    return insertResults(assessmentId, item.getResults());
}

std::string AssessmentStorage::getUpdateCommand() const
{
    return "UPDATE assessment SET name = ?, testType_id = ?, subject_id = ?, "
           "class_id = ?, date = ?, maxScore = ? WHERE id = ?";
}

std::vector<std::string> AssessmentStorage::getUpdateValues(const Assessment &item) const
{
    auto assessmentDateTime = SQLiteDateTimeFactory::NewDateTimeFromPTime(item.getDateTime());
    return { boost::trim_copy(item.getName()),
             to_string(item.getTestType().getId()),
             to_string(item.getSubject().getId()),
             to_string(item.getClass().getId()),
             assessmentDateTime.toSQLiteString(),
             to_string(item.getMaxScore()),
             to_string(item.getId()) 
           };
}

bool AssessmentStorage::preUpdateStep(const Assessment &item) 
{ 
    //Load old results
   oldResults = map<size_t, vector<AssessmentResult>>(); 
    try {
        oldResults = loadAllResults(fmt::format("WHERE class.id = {0} ", item.getClass().getId()));
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return false;
    }
    return true;
}

bool AssessmentStorage::postUpdateStep(const Assessment &item) 
{
    vector<size_t> assessmentResultIdsToRemove;
    vector<size_t> assessmentResultIdsToUpdate;
    for(const auto &oldResult : oldResults[item.getId()]) {
        if (find(item.getResults().begin(), item.getResults().end(), oldResult) == item.getResults().end()) {
            assessmentResultIdsToRemove.emplace_back(oldResult.getId());
        } 
        else {
            assessmentResultIdsToUpdate.emplace_back(oldResult.getId());
        }
    }
    //Update existing results
    for(const auto &result : item.getResults()) {
        if (find(assessmentResultIdsToUpdate.begin(), assessmentResultIdsToUpdate.end(), result.getId()) != assessmentResultIdsToUpdate.end()) {
            if (!updateResult(result)) {
                return false;
            }
        }
    }
    //Remove old results
    if (assessmentResultIdsToRemove.size() > 0 && !removeResults(assessmentResultIdsToRemove)) {
        return false;
    }
    //Add new results
    vector<AssessmentResult> assessmentResultsToAdd;
    for(const auto &result : item.getResults()) {
        if (result.getId() == 0) {
            assessmentResultsToAdd.emplace_back(result);
        }
    }
    return insertResults(item.getId(), assessmentResultsToAdd);
}

std::string AssessmentStorage::getDeleteCommand() const
{
    return "DELETE FROM assessment WHERE id = ?";
}

std::vector<std::string> AssessmentStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

QueryResult AssessmentStorage::preDeleteStep(size_t id) 
{
    //Load the assessment to delete
    auto assessment = getItemById(id);
    if (!assessment) {
        return QueryResult::ERROR;
    }

    //Delete all the results of the assessment
    vector<size_t> resultIdsToRemove;
    transform(assessment->getResults().begin(), assessment->getResults().end(), std::back_inserter(resultIdsToRemove),
               [](AssessmentResult const& x) { return x.getId(); });
    if (resultIdsToRemove.size() > 0 && !removeResults(resultIdsToRemove)) {
        return QueryResult::ERROR;
    }
    return QueryResult::OK;
}

bool AssessmentStorage::isReferentialIntegrityConstraint(size_t id)
{
    return false;
}

std::string AssessmentStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "";
}

std::vector<std::string> AssessmentStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return {};
}

bool AssessmentStorage::insertResults(size_t assessmentId, const vector<AssessmentResult> &resultsToAdd)
{
    if (resultsToAdd.size() > 0) {
        //Insert all results
        string sqlQueryInsertResults {"INSERT INTO assessmentResult (assessment_id, student_id, result, comments) VALUES" };
        vector<string> resultValues;
        size_t i {0};
        for (const auto &result : resultsToAdd) {
            sqlQueryInsertResults += " (?, ?, ?, ?)";
            resultValues.emplace_back(to_string(assessmentId));
            resultValues.emplace_back(to_string(result.getStudent().getId()));
            resultValues.emplace_back(to_string(result.getResult()));
            resultValues.emplace_back(result.getComments());
            i++;
            if (i < resultsToAdd.size()) {
                sqlQueryInsertResults += ",";
            }
        }
        auto operationInsertResults = operationFactory->createInsertOperation(*connection, sqlQueryInsertResults, resultValues);
        if (!operationInsertResults->execute()) {
            lastError = operationInsertResults->getLastError();
            return false;
        }
    }
    return true;
}

bool AssessmentStorage::updateResult(const AssessmentResult &resultToUpdate) 
{
    auto operationUpdateResult = operationFactory->createUpdateOperation(*connection, 
        "UPDATE assessmentResult SET result = ?, comments = ? WHERE id = ?", 
        vector<string> { to_string(resultToUpdate.getResult()),
                            resultToUpdate.getComments(),
                            to_string(resultToUpdate.getId()) });
    if (!operationUpdateResult->execute()) {
        lastError = operationUpdateResult->getLastError();
        return false;
    }
    return true;
}

bool AssessmentStorage::removeResults(const vector<size_t> &assessmentResultIdsToRemove)
{
    string sqlQueryRemoveResults {"DELETE FROM assessmentResult WHERE" };
    for (size_t i = 0; i < assessmentResultIdsToRemove.size(); i++)
    {
        if (i > 0) {
           sqlQueryRemoveResults += " OR";
        }
        sqlQueryRemoveResults += " id = ?";
    }

    vector<string> ids;
    transform(assessmentResultIdsToRemove.begin(), assessmentResultIdsToRemove.end(), back_inserter(ids),
              [](size_t id) {
                  return to_string(id);
              });
    auto operation = operationFactory->createDeleteOperation(*connection, sqlQueryRemoveResults, ids);
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}

map<size_t, vector<AssessmentResult>> AssessmentStorage::loadAllResults(const string &whereClause) 
{
    map<size_t, vector<AssessmentResult>> assessmentResults;
    auto operationLoadResults = operationFactory->createSelectOperation(*connection, 
        "SELECT assessment_id, assessmentResult.id, result, assessmentResult.comments, student_id, student.firstname, student.lastname, student.comments "
        "FROM assessmentResult "
        "INNER JOIN Student ON assessmentResult.student_id = student.id "
        "INNER JOIN assessment ON assessmentResult.assessment_id = assessment.id "
        "INNER JOIN class on assessment.class_id = class.id " +
        whereClause);
    if (operationLoadResults->execute()) {
        while (operationLoadResults->getRow()) {
            assessmentResults[operationLoadResults->getIntValue(0)].emplace_back(
                operationLoadResults->getIntValue(1),
                    Student(operationLoadResults->getIntValue(4),
                        operationLoadResults->getStringValue(5),
                        operationLoadResults->getStringValue(6),
                        operationLoadResults->getStringValue(7)),
                    static_cast<float>(operationLoadResults->getDoubleValue(2)),
                    operationLoadResults->getStringValue(3));
        }
        operationLoadResults->close();
    }
    else {
        throw runtime_error(operationLoadResults->getLastError());
    }
    return assessmentResults;
}
