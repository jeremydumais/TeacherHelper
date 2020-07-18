#include "classController.h"
#include "classStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

ClassController::ClassController(const DatabaseConnection &dbConnection,
                               unique_ptr<IManagementItemStorage<Class>> managementItemStorage)
    : classes(list<Class>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<IManagementItemStorage<Class>>(make_unique<ClassStorage>(dbConnection))},
      lastError("")
{
}

const std::list<Class> &ClassController::getClasses() const
{
    return classes;
}

const Class *ClassController::findClass(size_t id) const
{
	const Class *retVal = nullptr;
	for(const auto &aClass : classes) {
		if (aClass.getId() == id) {
			retVal = &aClass; 
			break;
		}
	}
	return retVal;
}

bool ClassController::isNewNameAvailableForAdd(const string &name, 
                                               const size_t school_id) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

    bool problemFound = false;
	for(const auto &aClass : classes) {
		if (boost::to_upper_copy(boost::trim_copy(aClass.getName())) == boost::to_upper_copy(boost::trim_copy(name))
            && aClass.getSchool().getId() == school_id) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

bool ClassController::isNewNameAvailableForUpdate(const string &name,
                                                  const size_t school_id,
								 				  size_t currentlyEditedClassId) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

	bool problemFound = false;
	for(const auto &aClass : classes) {
		if (boost::to_upper_copy(boost::trim_copy(aClass.getName())) == boost::to_upper_copy(boost::trim_copy(name)) 
            && aClass.getSchool().getId() == school_id
            && aClass.getId() != currentlyEditedClassId) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

const std::string &ClassController::getLastError() const
{
    return lastError;
}

void ClassController::loadClasses()
{
	classes = storage->getAllItems();
}

bool ClassController::insertClass(const Class &p_class)
{
    bool retVal = storage->insertItem(p_class);
    if (retVal) {
        loadClasses();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool ClassController::updateClass(const Class &p_class)
{
    bool retVal = storage->updateItem(p_class);
    if (retVal) {
        loadClasses();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool ClassController::deleteClass(size_t id)
{
    bool retVal = storage->deleteItem(id);
    if (retVal) {
        loadClasses();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}
