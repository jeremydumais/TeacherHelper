#include "assessment.h"
#include "boost/algorithm/string.hpp"
#include <algorithm>

using namespace std;
using namespace boost;
using namespace boost::posix_time;

Assessment::Assessment(const string &name, 
           const TestType &testType,
           const Subject &subject,
           const Class &itemClass,
           const boost::posix_time::ptime &itemDate)
    : Assessment(0, name, testType, subject, itemClass, itemDate)
{  
}

Assessment::Assessment(size_t id,
           const string &name, 
           const TestType &testType,
           const Subject &subject,
           const Class &itemClass,
           const boost::posix_time::ptime &itemDate)
    : id(id), 
      name(name),
      testType(testType),
      subject(subject),
      itemClass(itemClass),
      itemDate(itemDate),
      results(vector<AssessmentResult>())
{    
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 100) {
        throw invalid_argument("name must not be larger then 100 chars.");
    }
}

size_t Assessment::getId() const
{
    return id;
}

const std::string &Assessment::getName() const
{
    return name;
}

const TestType& Assessment::getTestType() const
{
    return testType;
}

const Subject& Assessment::getSubject() const
{
    return subject;
}

const Class& Assessment::getClass() const
{
    return itemClass;
}

const boost::posix_time::ptime& Assessment::getDateTime() const
{
    return itemDate;
}

const std::vector<AssessmentResult>& Assessment::getResults() const
{
    return results;
}


void Assessment::setName(const std::string &name) 
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 100) {
        throw invalid_argument("name must not be larger then 100 chars.");
    }   
    this->name = name;
}

void Assessment::setTestType(const TestType &testType) 
{
    this->testType = testType;
}

void Assessment::setSubject(const Subject &subject) 
{
    this->subject = subject;
}

void Assessment::setClass(const Class &itemClass) 
{
    this->itemClass = itemClass;
}

void Assessment::setDate(const ptime &itemDate) 
{
    this->itemDate = itemDate;
}

void Assessment::addResult(const AssessmentResult &assessmentResult) 
{
    //Ensure the assessment result is not already in the vector
    auto resultIterator = find_if(results.cbegin(),
                               results.cend(),
                               [assessmentResult](const AssessmentResult &result) {
                                   return assessmentResult.getStudent() ==  result.getStudent();
                               });
    if (resultIterator!=results.cend())
        throw invalid_argument("Cannot add the same assessment result twice.");
    results.emplace_back(assessmentResult);
}


void Assessment::removeResult(const AssessmentResult &result) 
{
    //Ensure the student is in the list
    auto resultIterator = find(results.cbegin(),
                               results.cend(),
                               result);
    if (resultIterator!=results.cend()) {
        results.erase(resultIterator);
    }
    else {
        throw invalid_argument("That result is not part of the list.");
    }
}

void Assessment::clearResults() 
{
    results.clear();
}