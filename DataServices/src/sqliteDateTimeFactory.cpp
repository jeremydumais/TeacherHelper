#include "sqliteDateTimeFactory.h"

SQLiteDateTime SQLiteDateTimeFactory::NewDateTimeFromPTime(const boost::posix_time::ptime &datetime) 
{
    return SQLiteDateTime(datetime);
}
