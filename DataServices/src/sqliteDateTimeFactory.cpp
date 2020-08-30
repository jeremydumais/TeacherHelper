#include "sqliteDateTimeFactory.h"
#include <boost/date_time/posix_time/time_parsers.hpp>

using namespace boost::posix_time;

SQLiteDateTime SQLiteDateTimeFactory::NewDateTimeFromPTime(const boost::posix_time::ptime &datetime) 
{
    return SQLiteDateTime(datetime);
}

SQLiteDateTime SQLiteDateTimeFactory::NewDateTimeFromISOExtended(const std::string &textDateTime) 
{
    return SQLiteDateTime(from_iso_extended_string(textDateTime));
}
