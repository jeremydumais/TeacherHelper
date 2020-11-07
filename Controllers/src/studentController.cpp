#include "studentController.h"
#include "studentStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

StudentController::StudentController(const IDatabaseController &databaseController,
                               unique_ptr<ManagementItemStorageBase<Student>> managementItemStorage)
    : students(list<Student>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<Student>>(make_unique<StudentStorage>(databaseController.getDatabaseConnection()))},
      lastError("")
{
}

const std::list<Student> &StudentController::getStudents() const
{
    return students;
}

const Student *StudentController::findStudent(size_t id) const
{
	const Student *retVal = nullptr;
	for(const auto &student : students) {
		if (student.getId() == id) {
			retVal = &student; 
			break;
		}
	}
	return retVal;
}

const std::string &StudentController::getLastError() const
{
    return lastError;
}

void StudentController::loadStudents()
{
	students = storage->getAllItems();
}

bool StudentController::insertStudent(const Student &student)
{
    bool retVal = storage->insertItem(student);
    if (retVal) {
        loadStudents();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool StudentController::updateStudent(const Student &student)
{
    bool retVal = storage->updateItem(student);
    if (retVal) {
        loadStudents();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool StudentController::deleteStudent(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadStudents();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the student because it is used by another item. (Probably a class or an assessment result)";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}
