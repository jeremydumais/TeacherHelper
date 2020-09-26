#pragma once

#include <string>
#include "city.h"

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define SCHOOL_API __declspec(dllexport)   
    #else  
        #define SCHOOL_API __declspec(dllimport)   
    #endif
#else
    #define SCHOOL_API
#endif

class SCHOOL_API School
{
public:
    School(const std::string &name, 
            const City &city);
    School(size_t id,
            const std::string &name, 
            const City &city);
    SCHOOL_API friend bool operator==(const School& lhs, const School &rhs);
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