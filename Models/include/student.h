#pragma once

#include <string>

class Student
{
public:
    Student(const std::string &firstName, 
            const std::string &lastName,
            const std::string &comments = "");
    Student(size_t id,
            const std::string &firstName, 
            const std::string &lastName,
            const std::string &comments = "");

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

