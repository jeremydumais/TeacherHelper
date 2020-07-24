#include "student.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

Student::Student(const string &firstName, 
                 const string &lastName,
                 const string &comments)
    : Student(0, firstName, lastName, comments)
{  
}

Student::Student(size_t id,
            const std::string &firstName, 
            const std::string &lastName,
            const std::string &comments)
    : id(id),
      firstName(firstName),
      lastName(lastName),
      comments(comments)
{    
    if (trim_copy(firstName).empty()) {
        throw invalid_argument("firstName cannot be null or empty.");
    }
    if (firstName.length() > 30) {
        throw invalid_argument("firstName must not be larger then 30 chars.");
    }    
    if (trim_copy(lastName).empty()) {
        throw invalid_argument("lastName cannot be null or empty.");
    }
    if (lastName.length() > 30) {
        throw invalid_argument("lastName must not be larger then 30 chars.");
    }
    if (comments.length() > 256) {
        throw invalid_argument("comments must not be larger then 256 chars.");
    }
}

bool operator==(const Student& lhs, const Student &rhs)
{
    return lhs.getId() == rhs.getId();
}

size_t Student::getId() const
{
    return id;
}

const std::string &Student::getFirstName() const
{
    return firstName;
}

const std::string &Student::getLastName() const
{
    return lastName;
}

const std::string& Student::getComments() const
{
    return comments;
}

void Student::setFirstName(const std::string &firstName) 
{
    if (trim_copy(firstName).empty()) {
        throw invalid_argument("firstName cannot be null or empty.");
    }
    if (firstName.length() > 30) {
        throw invalid_argument("firstName must not be larger then 30 chars.");
    } 
    this->firstName = firstName;  
}

void Student::setLastName(const std::string &lastName) 
{
    if (trim_copy(lastName).empty()) {
        throw invalid_argument("lastName cannot be null or empty.");
    }
    if (lastName.length() > 30) {
        throw invalid_argument("lastName must not be larger then 30 chars.");
    }
    this->lastName = lastName;
}

void Student::setComments(const std::string &comments) 
{
    if (comments.length() > 256) {
        throw invalid_argument("comments must not be larger then 256 chars.");
    }
    this->comments = comments;
}