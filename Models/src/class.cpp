#include "class.h"
#include "boost/algorithm/string.hpp"

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
