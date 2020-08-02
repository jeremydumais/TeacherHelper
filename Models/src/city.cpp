#include "city.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

City::City(const string &name)
    : City(0, name)
{  
}

City::City(size_t id,
            const std::string &name)
    : id(id),
      name(name)
{    
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
}

bool operator==(const City& lhs, const City &rhs)
{
    return lhs.id == rhs.id &&
           lhs.name == rhs.name;
}

size_t City::getId() const
{
    return id;
}

const std::string &City::getName() const
{
    return name;
}

void City::setName(const string &name)
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
    this->name = name; 
}
