#pragma once

#include "assessment.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
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
    explicit AssessmentController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<Assessment>> managementItemStorage = nullptr);
    const std::list<Assessment> &getAssessments() const;
    const Assessment* findAssessment(size_t id) const;
    const std::string &getLastError() const;
    void loadAssessments();
    bool insertAssessment(const Assessment &assessment);
    bool updateAssessment(const Assessment &assessment);
    bool deleteAssessment(size_t id);
private:
	std::list<Assessment> assessments;
    std::unique_ptr<IManagementItemStorage<Assessment>> storage;
    std::string lastError;
};