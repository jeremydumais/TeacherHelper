#pragma once

#include <string>

class Student
{
public:
    Student(const std::string &firstName, 
            const std::string &lastName);
    Student(size_t id,
            const std::string &firstName, 
            const std::string &lastName);

    size_t getId() const;
    const std::string &getFirstName() const;
    const std::string &getLastName() const;
private:
    size_t id;
    std::string firstName;
    std::string lastName;
};