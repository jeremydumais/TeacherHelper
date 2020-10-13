#pragma once

#include <string>

#ifdef _WIN32
    #ifdef MODELS_EXPORTS  
        #define VERSION_API __declspec(dllexport)   
    #else  
        #define VERSION_API __declspec(dllimport)   
    #endif
#else
    #define VERSION_API
#endif

class VERSION_API Version
{
public:
    Version(unsigned char major, unsigned char minor, unsigned char patch);
    unsigned char getMajor() const;
private:
    unsigned char major;
    unsigned char minor;
    unsigned char patch;
};