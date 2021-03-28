#pragma once

#include "assessment.h"
#include "IDatabaseController.h"
#include "ManagementItemStorageBase.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define ASSESSMENTCONTROLLER_API __declspec(dllexport)   
    #else  
        #define ASSESSMENTCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define ASSESSMENTCONTROLLER_API
#endif

class ASSESSMENTCONTROLLER_API AssessmentController
{
public:
    explicit AssessmentController(const IDatabaseController &databaseController,
                            std::unique_ptr<ManagementItemStorageBase<Assessment>> managementItemStorage = nullptr);
    const std::list<Assessment> &getAssessments() const;
    const Assessment* findAssessment(size_t id) const;
    const std::string &getLastError() const;
    void loadAssessments();
    void loadAssessmentsByClass(const size_t classId);
    bool insertAssessment(const Assessment &assessment);
    bool updateAssessment(const Assessment &assessment);
    bool deleteAssessment(size_t id);
    std::vector<AssessmentResult> getStudentAllAssessmentResults(const Student &student) const;
private:
	std::list<Assessment> assessments;
    std::unique_ptr<ManagementItemStorageBase<Assessment>> storage;
    std::string lastError;
};