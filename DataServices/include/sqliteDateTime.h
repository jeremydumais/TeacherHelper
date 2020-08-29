#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SQLITEDATETIME_API __declspec(dllexport)   
    #else  
        #define SQLITEDATETIME_API __declspec(dllimport)   
    #endif
#else
    #define SQLITEDATETIME_API
#endif

class SQLITEDATETIME_API SQLiteDateTime
{
public:
    const boost::posix_time::ptime &getBoostPTime() const;
	SQLITEDATETIME_API friend bool operator==(const SQLiteDateTime& lhs, const SQLiteDateTime &rhs);
    SQLITEDATETIME_API friend bool operator!=(const SQLiteDateTime& lhs, const SQLiteDateTime &rhs);
    SQLITEDATETIME_API friend bool operator<(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs);
	SQLITEDATETIME_API friend bool operator>(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs);
	SQLITEDATETIME_API friend bool operator<=(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs);
	SQLITEDATETIME_API friend bool operator>=(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs);
    friend class SQLiteDateTimeFactory;
private:
    boost::posix_time::ptime datetime;
    SQLiteDateTime() = delete;
    SQLiteDateTime(const boost::posix_time::ptime &datetime);
};