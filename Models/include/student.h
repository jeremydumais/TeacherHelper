#pragma once

#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define STUDENT_API __declspec(dllexport)   
    #else  
        #define STUDENT_API __declspec(dllimport)   
    #endif
#else
    #define STUDENT_API
#endif

class STUDENT_API Student
{
public:
    Student(const std::string &firstName, 
            const std::string &lastName,
            const std::string &comments = "");
    Student(size_t id,
            const std::string &firstName, 
            const std::string &lastName,
            const std::string &comments = "");
    STUDENT_API friend bool operator==(const Student& lhs, const Student &rhs);
    size_t getId() const;
    const std::string &getFirstName() const;
    const std::string &getLastName() const;
    const std::string &getComments() const;
    void setFirstName(const std::string &firstName);
    void setLastName(const std::string &lastName);
    void setComments(const std::string &comments);
private:
    size_t id;
    std::string firstName;
    std::string lastName;
    std::string comments;
};

