#pragma once

#include <string>

class School
{
public:
    School(const std::string &name, 
            const std::string &city);
    School(size_t id,
            const std::string &name, 
            const std::string &city);

    size_t getId() const;
    const std::string &getName() const;
    const std::string &getCity() const;
private:
    size_t id;
    std::string name;
    std::string city;
};