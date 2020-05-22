#pragma once

#include "databaseConnection.h"
#include "student.h"
#include <list>

class StudentStorage
{
public:
    explicit StudentStorage(const DatabaseConnection &connection);
    std::list<Student> getAllStudents();
    const std::string &getLastError() const;
    bool insertStudent(const Student &student);
    bool updateStudent(const Student &student);
    bool deleteStudent(size_t id);
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};