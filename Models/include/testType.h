#pragma once

#include <string>

class TestType
{
public:
    explicit TestType(const std::string &name);
    TestType(size_t id,
            const std::string &name);

    size_t getId() const;
    const std::string &getName() const;
    void setName(const std::string &name);
private:
    size_t id;
    std::string name;
};