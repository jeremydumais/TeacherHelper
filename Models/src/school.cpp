#include "school.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

School::School(const string &name, 
            const string &city)
    : School(0, name, city)
{  
}

School::School(size_t id,
            const std::string &name, 
            const std::string &city)
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
    if (trim_copy(city).empty()) {
        throw invalid_argument("city cannot be null or empty.");
    }
    if (city.length() > 50) {
        throw invalid_argument("city must not be larger then 50 chars.");
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

const std::string &School::getCity() const
{
    return city;
}
