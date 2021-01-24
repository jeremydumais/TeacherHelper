#pragma once

#include "assessmentResult.h"
#include "class.h"
#include "subject.h"
#include "testType.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <vector>
#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define ASSESSMENT_API __declspec(dllexport)   
    #else  
        #define ASSESSMENT_API __declspec(dllimport)   
    #endif
#else
        #define ASSESSMENT_API
#endif

class ASSESSMENT_API Assessment
{
public:
    Assessment(const std::string &name, 
            const TestType &testType,
            const Subject &subject,
            const Class &itemClass,
            const boost::posix_time::ptime &itemDate,
            float maxScore = 100.0f);
    Assessment(size_t id,
            const std::string &name, 
            const TestType &testType,
            const Subject &subject,
            const Class &itemClass,
            const boost::posix_time::ptime &itemDate,
            float maxScore = 100.0f);
    size_t getId() const;
    const std::string &getName() const;
    const TestType &getTestType() const;
    const Subject &getSubject() const;
    const Class &getClass() const;
    const boost::posix_time::ptime &getDateTime() const;
    float getMaxScore() const;
    const std::vector<AssessmentResult> &getResults() const;
    void setName(const std::string &name);
    void setTestType(const TestType &testType);
    void setSubject(const Subject &subject);
    void setClass(const Class &itemClass);
    void setDate(const boost::posix_time::ptime &itemDate);
    void setMaxScore(float maxScore);
    void addResult(const AssessmentResult &assessmentResult);
    void removeResult(const AssessmentResult &result);
    void clearResults();
private:
    size_t id;
    std::string name;
    TestType testType;
    Subject subject;
    Class itemClass;
    boost::posix_time::ptime itemDate;
    float maxScore;
    std::vector<AssessmentResult> results;
};




