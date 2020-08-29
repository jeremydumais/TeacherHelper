#include "sqliteDateTimeFactory.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

TEST(SQLiteDateTimeFactory_NewDateTimeFromPTime, BeginOf2020_ReturnValidDateTime)
{
    auto expected = ptime(date(2020,01,01), time_duration(0,0,0));
    auto actual = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2020,01,01), time_duration(0,0,0)));
    ASSERT_EQ(expected, actual.getBoostPTime());
}