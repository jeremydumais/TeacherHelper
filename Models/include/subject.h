#pragma once

#include <string>

class Subject
{
public:
    explicit Subject(const std::string &name,
                     bool isDefault = false);
    Subject(size_t id,
            const std::string &name,
            bool isDefault = false);

    size_t getId() const;
    const std::string &getName() const;
    bool getIsDefault() const;
    void setName(const std::string &name);
    void setIsDefault(bool isDefault);
private:
    size_t id;
    std::string name;
    bool isDefault;
};
