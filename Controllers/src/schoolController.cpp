#include "schoolController.h"
#include "schoolStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

SchoolController::SchoolController(const IDatabaseController &databaseController,
                               unique_ptr<ManagementItemStorageBase<School>> managementItemStorage)
    : schools(list<School>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<School>>(make_unique<SchoolStorage>(databaseController.getDatabaseConnection()))},
      lastError("")
{
}

const std::list<School> &SchoolController::getSchools() const
{
    return schools;
}

const School *SchoolController::findSchool(size_t id) const
{
	const School *retVal = nullptr;
	for(const auto &school : schools) {
		if (school.getId() == id) {
			retVal = &school; 
			break;
		}
	}
	return retVal;
}

bool SchoolController::isNewNameAvailableForAdd(const string &name) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

    bool problemFound = false;
	for(const auto &school : schools) {
		if (boost::to_upper_copy(boost::trim_copy(school.getName())) == boost::to_upper_copy(boost::trim_copy(name))) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

bool SchoolController::isNewNameAvailableForUpdate(const string &name,
								 					 size_t currentlyEditedSchoolId) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

	bool problemFound = false;
	for(const auto &school : schools) {
		if (boost::to_upper_copy(boost::trim_copy(school.getName())) == boost::to_upper_copy(boost::trim_copy(name)) && school.getId() != currentlyEditedSchoolId) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

const std::string &SchoolController::getLastError() const
{
    return lastError;
}

void SchoolController::loadSchools()
{
	schools = storage->getAllItems();
}

bool SchoolController::insertSchool(const School &school)
{
    bool retVal = storage->insertItem(school);
    if (retVal) {
        loadSchools();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool SchoolController::updateSchool(const School &school)
{
    bool retVal = storage->updateItem(school);
    if (retVal) {
        loadSchools();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool SchoolController::deleteSchool(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadSchools();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the school because it is used by another item. (Probably a class)";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}
