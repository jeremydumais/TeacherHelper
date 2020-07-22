#pragma once

#include "student.h"
#include "studentController.h"
#include "databaseConnection.h"
#include <list>

class StudentSelectionController
{
public:
    explicit StudentSelectionController(const DatabaseConnection &dbConnection);
	bool isStudentInFilter(const std::string &filter, const Student &student) const;
    const std::list<Student> &getStudents() const;
    const Student* findStudent(size_t id) const;
    void loadStudents();
private:
    StudentController studentController;
};