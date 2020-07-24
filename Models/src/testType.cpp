#include "testType.h"
#include "boost/algorithm/string.hpp"

using namespace std;
using namespace boost;

TestType::TestType(const string &name)
    : TestType(0, name)
{  
}

TestType::TestType(size_t id,
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

size_t TestType::getId() const
{
    return id;
}

const std::string &TestType::getName() const
{
    return name;
}

void TestType::setName(const string &name)
{
    if (trim_copy(name).empty()) {
        throw invalid_argument("name cannot be null or empty.");
    }
    if (name.length() > 50) {
        throw invalid_argument("name must not be larger then 50 chars.");
    }    
    this->name = name; 
}
