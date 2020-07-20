#include "subject.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

Subject::Subject(const string &name,
                 bool isDefault)
    : Subject(0, name, isDefault)
{  
}

Subject::Subject(size_t id,
                 const std::string &name,
                 bool isDefault)
    : id(id),
      name(name),
      isDefault(isDefault)
{    
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
}

size_t Subject::getId() const
{
    return id;
}

const std::string &Subject::getName() const
{
    return name;
}

bool Subject::getIsDefault() const
{
    return isDefault;
}

void Subject::setName(const string &name)
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
    this->name = name; 
}

void Subject::setIsDefault(bool isDefault) 
{
    this->isDefault = isDefault;
}
