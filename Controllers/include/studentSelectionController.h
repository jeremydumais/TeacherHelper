#pragma once

#include "student.h"
#include "studentController.h"
#include "databaseConnection.h"
#include <list>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define STUDENTSELECTIONCONTROLLER_API __declspec(dllexport)   
    #else  
        #define STUDENTSELECTIONCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define STUDENTSELECTIONCONTROLLER_API
#endif

class STUDENTSELECTIONCONTROLLER_API StudentSelectionController
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