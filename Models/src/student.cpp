#include "student.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

Student::Student(const string &firstName, 
            const string &lastName)
    : Student(0, firstName, lastName)
{  
}

Student::Student(size_t id,
            const std::string &firstName, 
            const std::string &lastName)
    : id(id),
      firstName(firstName),
      lastName(lastName)
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
