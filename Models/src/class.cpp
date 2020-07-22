#include "class.h"
#include "boost/algorithm/string.hpp"
#include <algorithm>

using namespace std;
using namespace boost;

Class::Class(const string &name, 
            const School &school)
    : Class(0, name, school)
{  
}

Class::Class(size_t id,
            const std::string &name, 
            const School &school)
    : id(id), 
      name(name),
      school(school)
{    
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
}

size_t Class::getId() const
{
    return id;
}

const std::string &Class::getName() const
{
    return name;
}

const School &Class::getSchool() const
{
    return school;
}

const std::list<Student>& Class::getMembers() const
{
    return members;
}

void Class::setName(const std::string &name) 
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }   
    this->name = name;
}

void Class::setSchool(const School &school) 
{
    this->school = school;
}

void Class::addMember(const Student &student) 
{
    //Ensure the student is not already in the list
    auto resultIterator = find(members.cbegin(),
                               members.cend(),
                               student);
    if (resultIterator!=members.cend())
        throw invalid_argument("Cannot add the same member twice.");
    members.emplace_back(student);
}

void Class::removeMember(const Student &student) 
{
    //Ensure the student is in the list
    auto resultIterator = find(members.cbegin(),
                               members.cend(),
                               student);
    if (resultIterator!=members.cend()) {
        members.remove(student);
    }
    else {
        throw invalid_argument("That student is not part of the list.");
    }
}
