#include "subjectController.h"
#include "subjectStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

SubjectController::SubjectController(const DatabaseConnection &dbConnection,
                               unique_ptr<ManagementItemStorageBase<Subject>> managementItemStorage)
    : subjects(list<Subject>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<Subject>>(make_unique<SubjectStorage>(dbConnection))},
      lastError("")
{
}

const std::list<Subject> &SubjectController::getSubjects() const
{
    return subjects;
}

const Subject *SubjectController::findSubject(size_t id) const
{
	const Subject *retVal = nullptr;
	for(const auto &subject : subjects) {
		if (subject.getId() == id) {
			retVal = &subject; 
			break;
		}
	}
	return retVal;
}

bool SubjectController::isNewNameAvailableForAdd(const string &name) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

    bool problemFound = false;
	for(const auto &subject : subjects) {
		if (boost::to_upper_copy(boost::trim_copy(subject.getName())) == boost::to_upper_copy(boost::trim_copy(name))) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

bool SubjectController::isNewNameAvailableForUpdate(const string &name,
								 					 size_t currentlyEditedSubjectId) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

	bool problemFound = false;
	for(const auto &subject : subjects) {
		if (boost::to_upper_copy(boost::trim_copy(subject.getName())) == boost::to_upper_copy(boost::trim_copy(name)) && subject.getId() != currentlyEditedSubjectId) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

const std::string &SubjectController::getLastError() const
{
    return lastError;
}

void SubjectController::loadSubjects()
{
	subjects = storage->getAllItems();
}

bool SubjectController::insertSubject(const Subject &subject)
{
    bool retVal = storage->insertItem(subject);
    if (retVal) {
        loadSubjects();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool SubjectController::updateSubject(const Subject &subject)
{
    bool retVal = storage->updateItem(subject);
    if (retVal) {
        loadSubjects();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool SubjectController::deleteSubject(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadSubjects();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the subject because it is used by another item. (Probably a test)";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}
