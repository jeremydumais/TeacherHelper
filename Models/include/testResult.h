#pragma once

#include <string>
#include "student.h"

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define TESTRESULT_API __declspec(dllexport)   
    #else  
        #define TESTRESULT_API __declspec(dllimport)   
    #endif
#else
    #define TESTRESULT_API
#endif

class TESTRESULT_API TestResult
{
public:
    TestResult(const Student &student, const float result, const std::string &comments = "");
    TestResult(size_t id, 
               const Student &student, 
               const float result, 
               const std::string &comments = "");
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