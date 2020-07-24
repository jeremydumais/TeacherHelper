#pragma once

#include <string>
#include "city.h"

class School
{
public:
    School(const std::string &name, 
            const City &city);
    School(size_t id,
            const std::string &name, 
            const City &city);

    size_t getId() const;
    const std::string &getName() const;
    const City &getCity() const;
    void setName(const std::string &name);
    void setCity(const City &city);
private:
    size_t id;
    std::string name;
    City city;
};