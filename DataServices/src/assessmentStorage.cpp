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

AssessmentStorage::AssessmentStorage(const DatabaseConnection &connection, 
                                     unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
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
        "SELECT assessment.id, assessment.name, testType.id, testType.name, subject.id, subject.name, " 
        "  class.id, class.name, school.id, school.name, city.id, city.name, date "
        "FROM assessment "
        "INNER JOIN testType ON testType.id = assessment.testType_id " 
        "INNER JOIN subject ON subject.id = assessment.subject_id " 
        "INNER JOIN class ON class.id = assessment.class_id " 
        "INNER JOIN school ON school.id = class.school_id " 
        "INNER JOIN city ON city.id = school.city_id " +
        whereClause +
        " ORDER BY datetime(date), assessment.name");
    if (operation->execute()) {
        while (operation->getRow()) {
            Assessment tempAssessment(operation->getIntValue(0),
                operation->getStringValue(1),
                TestType(
                    operation->getIntValue(2),
                    operation->getStringValue(3)),
                Subject(
                    operation->getIntValue(4),
                    operation->getStringValue(5)),
                Class(
                    operation->getIntValue(6),
                    operation->getStringValue(7),
                    School(
                    operation->getIntValue(8),
                    operation->getStringValue(9),
                    City(
                        operation->getIntValue(10),
                        operation->getStringValue(11)))),
                operation->getDateTime(12).getBoostPTime());
            retVal.push_back(tempAssessment);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
        return list<Assessment>();
    }
    //Load all results
    map<size_t, vector<AssessmentResult>> assessmentResults;
    try {
        assessmentResults = loadAllResults();
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return list<Assessment>();
    }
    //Associate all members to classes
    for(auto &assessment : retVal) {
        auto it = assessmentResults.find(assessment.getId());
        if (it != assessmentResults.end()) {
            for(const auto &result : it->second) {
                assessment.addResult(result);
            }
            it++;
        }
    }
    return retVal;
}

const string &AssessmentStorage::getLastError() const
{
    return lastError;
}

bool AssessmentStorage::insertItem(const Assessment &assessment)
{
    auto assessmentDateTime = SQLiteDateTimeFactory::NewDateTimeFromPTime(assessment.getDateTime());
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO assessment (name, testType_id, subject_id, class_id, date) "
        "VALUES(?, ?, ?, ?, ?)",
        vector<string> { boost::trim_copy(assessment.getName()),
                         to_string(assessment.getTestType().getId()), 
                         to_string(assessment.getSubject().getId()), 
                         to_string(assessment.getClass().getId()), 
                         assessmentDateTime.toSQLiteString()
                       });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }

    //Retreive the assigned primary key (id)
    size_t assessmentId = retreiveAssignedAssessmentId();
    if (assessmentId == 0) {
        return false;
    }

    return insertResults(assessmentId, assessment.getResults());
}

size_t AssessmentStorage::retreiveAssignedAssessmentId()
{
    size_t assessmentId {0};
    auto operationSelectAssignedId = operationFactory->createSelectOperation(*connection, 
        "SELECT last_insert_rowid()");
    if (operationSelectAssignedId->execute()) {
        if (operationSelectAssignedId->getRow()) {
            assessmentId = operationSelectAssignedId->getIntValue(0);
        }
        else {
            lastError = "Unable to retreive the assigned id for the new assessment record.";
        }
        operationSelectAssignedId->close();
    }
    else {
        lastError = operationSelectAssignedId->getLastError();
    }
    return assessmentId;
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

bool AssessmentStorage::updateItem(const Assessment &assessment)
{
    //Load old results
    map<size_t, vector<AssessmentResult>> oldResults;
    try {
        oldResults = loadAllResults(fmt::format("WHERE class.id = {0} ", assessment.getClass().getId()));
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return false;
    }

    auto assessmentDateTime = SQLiteDateTimeFactory::NewDateTimeFromPTime(assessment.getDateTime());
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE assessment SET name = ?, testType_id = ?, subject_id = ?, "
        "class_id = ?, date = ? WHERE id = ?",
        vector<string> { boost::trim_copy(assessment.getName()),
            to_string(assessment.getTestType().getId()),
            to_string(assessment.getSubject().getId()),
            to_string(assessment.getClass().getId()),
            assessmentDateTime.toSQLiteString(),
            to_string(assessment.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    vector<size_t> assessmentResultIdsToRemove;
    vector<size_t> assessmentResultIdsToUpdate;
    for(const auto &oldResult : oldResults[assessment.getId()]) {
        if (find(assessment.getResults().begin(), assessment.getResults().end(), oldResult) == assessment.getResults().end()) {
            assessmentResultIdsToRemove.emplace_back(oldResult.getId());
        } 
        else {
            assessmentResultIdsToUpdate.emplace_back(oldResult.getId());
        }
    }
    //Update existing results
    for(const auto &result : assessment.getResults()) {
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
    for(const auto &result : assessment.getResults()) {
        if (result.getId() == 0) {
            assessmentResultsToAdd.emplace_back(result);
        }
    }
    return insertResults(assessment.getId(), assessmentResultsToAdd);
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

QueryResult AssessmentStorage::deleteItem(size_t id)
{
    //Load the assessment to delete
    auto assessment = getItemById(id);
    if (!assessment) {
        return QueryResult::ERROR;
    }

    //Ensure that the record is not user as a foreign key in another table

    //Delete all the results of the assessment
    vector<size_t> resultIdsToRemove;
    transform(assessment->getResults().begin(), assessment->getResults().end(), std::back_inserter(resultIdsToRemove),
               [](AssessmentResult const& x) { return x.getId(); });
    if (resultIdsToRemove.size() > 0 && !removeResults(resultIdsToRemove)) {
        return QueryResult::ERROR;
    }
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM assessment WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
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
