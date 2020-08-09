#include "assessment.h"
#include "boost/algorithm/string.hpp"
#include <algorithm>

using namespace std;
using namespace boost;

Assessment::Assessment(const string &name, 
           const TestType &testType,
           const Subject &subject,
           const Class &itemClass)
    : Assessment(0, name, testType, subject, itemClass)
{  
}

Assessment::Assessment(size_t id,
           const string &name, 
           const TestType &testType,
           const Subject &subject,
           const Class &itemClass)
    : id(id), 
      name(name),
      testType(testType),
      subject(subject),
      itemClass(itemClass)
{    
    /*if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    } */   
}

size_t Assessment::getId() const
{
    return id;
}
/*
const std::string &Test::getName() const
{
    return name;
}

const School &Test::getSchool() const
{
    return school;
}

const std::list<Student>& Test::getMembers() const
{
    return members;
}

void Test::setName(const std::string &name) 
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }   
    this->name = name;
}

void Test::setSchool(const School &school) 
{
    this->school = school;
}

void Test::addMember(const Student &student) 
{
    //Ensure the student is not already in the list
    auto resultIterator = find(members.cbegin(),
                               members.cend(),
                               student);
    if (resultIterator!=members.cend())
        throw invalid_argument("Cannot add the same member twice.");
    members.emplace_back(student);
}

void Test::removeMember(const Student &student) 
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

void Test::clearMembers() 
{
    members.clear();
}

*/