#include "sqliteDateTime.h"
#include <fmt/format.h>

using namespace boost::posix_time;
using namespace boost::gregorian;

SQLiteDateTime::SQLiteDateTime(const boost::posix_time::ptime &datetime)
    : datetime(datetime)
{
}

const boost::posix_time::ptime &SQLiteDateTime::getBoostPTime() const
{
    return datetime;
}

std::string SQLiteDateTime::toSQLiteString() const
{
    return to_iso_extended_string(datetime);
}

bool operator==(const SQLiteDateTime& lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime == rhs.datetime;
}

bool operator!=(const SQLiteDateTime& lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime != rhs.datetime;
}

bool operator<(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime < rhs.datetime;
}

bool operator>(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime > rhs.datetime;
}

bool operator<=(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime <= rhs.datetime;
}

bool operator>=(const SQLiteDateTime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs.datetime >= rhs.datetime;
}

bool operator==(const SQLiteDateTime& lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime == rhs;
}

bool operator==(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs == rhs.datetime;
}

bool operator!=(const SQLiteDateTime &lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime != rhs;
}

bool operator!=(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs != rhs.datetime;
}

bool operator<(const SQLiteDateTime &lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime < rhs;
}

bool operator<(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return rhs.datetime >= lhs;
}

bool operator>(const SQLiteDateTime &lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime > rhs;
}

bool operator>(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs > rhs.datetime;
}

bool operator<=(const SQLiteDateTime &lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime <= rhs;
}

bool operator<=(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return rhs.datetime > lhs;
}

bool operator>=(const SQLiteDateTime &lhs, const boost::posix_time::ptime &rhs) 
{
    return lhs.datetime >= rhs;
}

bool operator>=(const boost::posix_time::ptime &lhs, const SQLiteDateTime &rhs) 
{
    return lhs >= rhs.datetime;
}