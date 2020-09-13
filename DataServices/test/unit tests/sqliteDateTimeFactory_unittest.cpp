#include "sqliteDateTimeFactory.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

TEST(SQLiteDateTimeFactory_NewDateTimeFromPTime, BeginOf2020_ReturnValidDateTime)
{
    auto expected = ptime(date(2020,01,01), time_duration(0,0,0));
    auto actual = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2020,01,01), time_duration(0,0,0)));
    ASSERT_EQ(expected, actual);
}

TEST(SQLiteDateTimeFactory_NewDateTimeFromISOExtended, ValidFormat_ReturnValidDateTime)
{
    auto expected = ptime(date(2020,8,29), time_duration(17,10,0));
    auto actual = SQLiteDateTimeFactory::NewDateTimeFromISOExtended("2020-08-29T17:10:00");
    ASSERT_EQ(expected, actual);
}

TEST(SQLiteDateTimeFactory_NewDateTimeFromISOExtended, InvalidFormat_ThrowOutOfRangeException)
{
    try
    {
        auto actual = SQLiteDateTimeFactory::NewDateTimeFromISOExtended("2020-08-29 17:10:00");
        FAIL();
    }
    catch(const std::out_of_range &e)
    {
    }
}

TEST(SQLiteDateTimeFactory_NewDateTimeFromISOExtended, EmptyString_ThrowOutOfRangeException)
{
    try
    {
        auto actual = SQLiteDateTimeFactory::NewDateTimeFromISOExtended("");
        FAIL();
    }
    catch(const std::out_of_range &e)
    {
    }
}

TEST(SQLiteDateTimeFactory_NewDateTimeFromISOExtended, InvalidDate_ThrowOutOfRangeException)
{
    try
    {
        auto actual = SQLiteDateTimeFactory::NewDateTimeFromISOExtended("2020-02-30T17:10:00");
        FAIL();
    }
    catch(const std::out_of_range &e)
    {
    }
}