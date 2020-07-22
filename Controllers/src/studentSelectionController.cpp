#include "studentSelectionController.h"
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;

StudentSelectionController::StudentSelectionController(const DatabaseConnection &dbConnection)
    : studentController(dbConnection)
{
}

bool StudentSelectionController::isStudentInFilter(const string &filter, const Student &student) const
{
	bool isFound {false};
	vector<string> filterWords;

	boost::split(filterWords, filter, boost::is_any_of(" "));
	if (filter == "") {
		isFound = true;
	}
	else {
		size_t wordFound {0};
		for(const auto &word : filterWords) {
			if (boost::contains(boost::to_upper_copy(student.getFirstName()), word) ||
				boost::contains(boost::to_upper_copy(student.getLastName()), word) ||
				boost::contains(boost::to_upper_copy(student.getComments()), word)) {
					wordFound++;
				}
		}
		if (wordFound == filterWords.size()) {
			isFound = true;
		}
	}
	return isFound;
} 

void StudentSelectionController::loadStudents()
{
    studentController.loadStudents();
}

const std::list<Student> &StudentSelectionController::getStudents() const
{
    return studentController.getStudents();
}

const Student* StudentSelectionController::findStudent(size_t id) const
{
    return studentController.findStudent(id);
}
