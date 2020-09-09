#pragma once

#include "student.h"
#include <list>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define ISTUDENTCONTROLLER_API __declspec(dllexport)   
    #else  
        #define ISTUDENTCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define ISTUDENTCONTROLLER_API
#endif

class ISTUDENTCONTROLLER_API IStudentController
{
public:
    virtual const std::list<Student> &getStudents() const = 0;
    virtual const Student* findStudent(size_t id) const = 0;
    virtual const std::string &getLastError() const = 0;
    virtual void loadStudents() = 0;
    virtual bool insertStudent(const Student &student) = 0;
    virtual bool updateStudent(const Student &student) = 0;
    virtual bool deleteStudent(size_t id) = 0;
};