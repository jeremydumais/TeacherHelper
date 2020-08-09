#pragma once

#include "class.h"
#include "subject.h"
#include "testType.h"
#include <list>
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
            const Class &itemClass);
    Assessment(size_t id,
            const std::string &name, 
            const TestType &testType,
            const Subject &subject,
            const Class &itemClass);
    size_t getId() const;
    /*const std::string &getName() const;
    const School &getSchool() const;
    const std::list<Student> &getMembers() const;
    void setName(const std::string &name);
    void setSchool(const School &school);
    void addMember(const Student &student);
    void removeMember(const Student &student);
    void clearMembers();*/
private:
    size_t id;
    std::string name;
    TestType testType;
    Subject subject;
    Class itemClass;
    /*School school;
    std::list<Student> members;*/
};




