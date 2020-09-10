#pragma once

#include "student.h"
#include <list>

class IStudentController
{
public:
    virtual ~IStudentController() {};
    virtual const std::list<Student> &getStudents() const = 0;
    virtual const Student* findStudent(size_t id) const = 0;
    virtual const std::string &getLastError() const = 0;
    virtual void loadStudents() = 0;
    virtual bool insertStudent(const Student &student) = 0;
    virtual bool updateStudent(const Student &student) = 0;
    virtual bool deleteStudent(size_t id) = 0;
};