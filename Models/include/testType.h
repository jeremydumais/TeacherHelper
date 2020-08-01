#pragma once

#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define TESTTYPE_API __declspec(dllexport)   
    #else  
        #define TESTTYPE_API __declspec(dllimport)   
    #endif
#else
    #define TESTTYPE_API
#endif

class TESTTYPE_API TestType
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