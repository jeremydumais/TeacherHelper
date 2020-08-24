#pragma once

#include <string>
#include "student.h"

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define ASSESSMENTRESULT_API __declspec(dllexport)   
    #else  
        #define ASSESSMENTRESULT_API __declspec(dllimport)   
    #endif
#else
    #define ASSESSMENTRESULT_API
#endif

class ASSESSMENTRESULT_API AssessmentResult
{
public:
    AssessmentResult(const Student &student, const float result, const std::string &comments = "");
    AssessmentResult(size_t id, 
               const Student &student, 
               const float result, 
               const std::string &comments = "");
    ASSESSMENTRESULT_API friend bool operator==(const AssessmentResult &lhs, const AssessmentResult &rhs);
    size_t getId() const;
    const Student &getStudent() const;
    const float getResult() const;
    const std::string &getComments() const;
    void setStudent(const Student &student);
    void setResult(const float result);
    void setComments(const std::string &comments);
private:
    size_t id;
    float result;
    Student student;
    std::string comments;
};