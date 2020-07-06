#pragma once

#include <string>

class City
{
public:
    City(const std::string &name);
    City(size_t id,
            const std::string &name);

    size_t getId() const;
    const std::string &getName() const;
    void setName(const std::string &name);
private:
    size_t id;
    std::string name;
};