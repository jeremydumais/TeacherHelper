#pragma once

#include <string>
#include "school.h"

class Class
{
public:
    Class(const std::string &name, 
            const School &school);
    Class(size_t id,
            const std::string &name, 
            const School &school);

    size_t getId() const;
    const std::string &getName() const;
    const School &getSchool() const;
private:
    size_t id;
    std::string name;
    School school;
};