#pragma once

#include "student.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class StudentController
{
public:
    explicit StudentController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<Student>> managementItemStorage = nullptr);
    const std::list<Student> &getStudents() const;
    const Student* findStudent(size_t id) const;
    const std::string &getLastError() const;
    void loadStudents();
    bool insertStudent(const Student &student);
    bool updateStudent(const Student &student);
    bool deleteStudent(size_t id);
private:
	std::list<Student> students;
    std::unique_ptr<IManagementItemStorage<Student>> storage;
    std::string lastError;
};