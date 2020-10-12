#include "assessmentController.h"
#include "assessmentStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

AssessmentController::AssessmentController(const DatabaseConnection &dbConnection,
                               unique_ptr<ManagementItemStorageBase<Assessment>> managementItemStorage)
    : assessments(list<Assessment>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<Assessment>>(make_unique<AssessmentStorage>(dbConnection))},
      lastError("")
{
}

const std::list<Assessment> &AssessmentController::getAssessments() const
{
    return assessments;
}

const Assessment *AssessmentController::findAssessment(size_t id) const
{
	const Assessment *retVal = nullptr;
	for(const auto &assessment : assessments) {
		if (assessment.getId() == id) {
			retVal = &assessment; 
			break;
		}
	}
	return retVal;
}

const std::string &AssessmentController::getLastError() const
{
    return lastError;
}

void AssessmentController::loadAssessments() 
{
	assessments = storage->getAllItems();
}

void AssessmentController::loadAssessmentsByClass(const size_t classId)
{
	auto assessmentStorage =  dynamic_cast<AssessmentStorage*>(storage.get());
    if (assessmentStorage) {
        assessments = assessmentStorage->getItemsByClassId(classId);
    }
}

bool AssessmentController::insertAssessment(const Assessment &assessment)
{
    bool retVal = storage->insertItem(assessment);
    if (retVal) {
        loadAssessments();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool AssessmentController::updateAssessment(const Assessment &assessment)
{
    bool retVal = storage->updateItem(assessment);
    if (retVal) {
        loadAssessments();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool AssessmentController::deleteAssessment(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadAssessments();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the assessment because it is used by another item.";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}

vector<AssessmentResult> AssessmentController::getStudentAllAssessmentResults(const Student &student) const
{
    vector<AssessmentResult> retVal;
    for(const auto &assessment : assessments) {
        copy_if(assessment.getResults().begin(), 
                assessment.getResults().end(), 
                back_inserter(retVal), 
                [&student] (const AssessmentResult &itemResult) { 
                    return itemResult.getStudent() == student;
                });
    }
    return retVal;
}
