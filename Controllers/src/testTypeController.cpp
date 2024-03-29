#include "testTypeController.h"
#include "testTypeStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

TestTypeController::TestTypeController(const IDatabaseController &databaseController,
                               unique_ptr<ManagementItemStorageBase<TestType>> managementItemStorage)
    : testTypes(list<TestType>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<TestType>>(make_unique<TestTypeStorage>(databaseController.getDatabaseConnection()))},
      lastError("")
{
}

const std::list<TestType> &TestTypeController::getTestTypes() const
{
    return testTypes;
}

const TestType *TestTypeController::findTestType(size_t id) const
{
	const TestType *retVal = nullptr;
	for(const auto &testType : testTypes) {
		if (testType.getId() == id) {
			retVal = &testType; 
			break;
		}
	}
	return retVal;
}

bool TestTypeController::isNewNameAvailableForAdd(const string &name) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

    bool problemFound = false;
	for(const auto &testType : testTypes) {
		if (boost::to_upper_copy(boost::trim_copy(testType.getName())) == boost::to_upper_copy(boost::trim_copy(name))) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

bool TestTypeController::isNewNameAvailableForUpdate(const string &name,
								 					 size_t currentlyEditedTestTypeId) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

	bool problemFound = false;
	for(const auto &testType : testTypes) {
		if (boost::to_upper_copy(boost::trim_copy(testType.getName())) == boost::to_upper_copy(boost::trim_copy(name)) && testType.getId() != currentlyEditedTestTypeId) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

const std::string &TestTypeController::getLastError() const
{
    return lastError;
}

void TestTypeController::loadTestTypes()
{
	testTypes = storage->getAllItems();
}

bool TestTypeController::insertTestType(const TestType &testType)
{
    bool retVal = storage->insertItem(testType);
    if (retVal) {
        loadTestTypes();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool TestTypeController::updateTestType(const TestType &testType)
{
    bool retVal = storage->updateItem(testType);
    if (retVal) {
        loadTestTypes();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool TestTypeController::deleteTestType(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadTestTypes();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the test type because it is used by another item. (Probably a test)";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}
