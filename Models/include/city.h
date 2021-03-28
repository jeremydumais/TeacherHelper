#pragma once

#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define CITY_API __declspec(dllexport) 
    #else  
        #define CITY_API __declspec(dllimport)   
    #endif
#else
    #define CITY_API
#endif

class CITY_API City
{
public:
    explicit City(const std::string &name);
    City(size_t id,
            const std::string &name);
    CITY_API friend bool operator==(const City& lhs, const City &rhs);
    size_t getId() const;
    const std::string &getName() const;
    void setName(const std::string &name);
private:
    size_t id;
    std::string name;
};