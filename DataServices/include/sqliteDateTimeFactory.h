#pragma once

#include "sqliteDateTime.h"

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SQLITEDATETIMEFACTORY_API __declspec(dllexport)   
    #else  
        #define SQLITEDATETIMEFACTORY_API __declspec(dllimport)   
    #endif
#else
    #define SQLITEDATETIMEFACTORY_API
#endif

class SQLITEDATETIMEFACTORY_API SQLiteDateTimeFactory
{
public:
    static SQLiteDateTime NewDateTimeFromPTime(const boost::posix_time::ptime &datetime);
    static SQLiteDateTime NewDateTimeFromISOExtended(const std::string &textDateTime);
};