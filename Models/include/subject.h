#pragma once

#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define SUBJECT_API __declspec(dllexport)   
    #else  
        #define SUBJECT_API __declspec(dllimport)   
    #endif
#else
    #define SUBJECT_API
#endif

class SUBJECT_API Subject
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
