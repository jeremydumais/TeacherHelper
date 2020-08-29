#include "sqliteDateTime.h"

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
