#pragma once

#include "IDatabaseController.h"
#include "IStudentController.h"
#include "ManagementItemStorageBase.h"
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define STUDENTCONTROLLER_API __declspec(dllexport)   
    #else  
        #define STUDENTCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define STUDENTCONTROLLER_API
#endif

class STUDENTCONTROLLER_API StudentController : public IStudentController
{
public:
    explicit StudentController(const IDatabaseController &databaseController,
                            std::unique_ptr<ManagementItemStorageBase<Student>> managementItemStorage = nullptr);
    const std::list<Student> &getStudents() const override;
    const Student* findStudent(size_t id) const override;
    const std::string &getLastError() const override;
    void loadStudents() override;
    bool insertStudent(const Student &student) override;
    bool updateStudent(const Student &student) override;
    bool deleteStudent(size_t id) override;
private:
	std::list<Student> students;
    std::unique_ptr<ManagementItemStorageBase<Student>> storage;
    std::string lastError;
};