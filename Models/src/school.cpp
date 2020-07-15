#include "school.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

School::School(const string &name, 
            const City &city)
    : School(0, name, city)
{  
}

School::School(size_t id,
            const string &name, 
            const City &city)
    : id(id),
      name(name),
      city(city)
{    
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
}

size_t School::getId() const
{
    return id;
}

const std::string &School::getName() const
{
    return name;
}

const City &School::getCity() const
{
    return city;
}

void School::setName(const std::string &name)
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }  
    this->name = name;
}

void School::setCity(const City &city)
{
    this->city = city;
}
