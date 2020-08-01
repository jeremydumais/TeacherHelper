#pragma once

#include "school.h"
#include "student.h"
#include <list>
#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define CLASS_API __declspec(dllexport)   
    #else  
        #define CLASS_API __declspec(dllimport)   
    #endif
#else
        #define CLASS_API
#endif

class CLASS_API Class
{
public:
    Class(const std::string &name, 
            const School &school);
    Class(size_t id,
            const std::string &name, 
            const School &school);

    size_t getId() const;
    const std::string &getName() const;
    const School &getSchool() const;
    const std::list<Student> &getMembers() const;
    void setName(const std::string &name);
    void setSchool(const School &school);
    void addMember(const Student &student);
    void removeMember(const Student &student);
    void clearMembers();
private:
    size_t id;
    std::string name;
    School school;
    std::list<Student> members;
};




