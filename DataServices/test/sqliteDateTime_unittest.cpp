#include "sqliteDateTimeFactory.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

TEST(SQLiteDateTime_GetBoostPTime, Begin2020_ReturnValidPTime)
{
    auto expected = ptime(date(2020,01,01), time_duration(0,0,0));
    auto actual = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2020,01,01), time_duration(0,0,0)));
    ASSERT_EQ(expected, actual.getBoostPTime());
}

TEST(SQLiteDateTime_GetBoostPTime, End2019_ReturnValidPTime)
{
    auto expected = ptime(date(2019,12,31), time_duration(23,59,59));
    auto actual = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_EQ(expected, actual.getBoostPTime());
}

TEST(SQLiteDateTime_OperatorCompareEqual, SameDate_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_EQ(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareEqual, OneSecondDiff_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    ASSERT_FALSE(date1 == date2);
}

TEST(SQLiteDateTime_OperatorCompareNotEqual, OneSecondDiff_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    ASSERT_NE(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareNotEqual, SameDate_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_FALSE(date1 != date2);
}

TEST(SQLiteDateTime_OperatorCompareLessThan, OneSecondDiff_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_LT(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareLessEqualThen, SameDate_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_FALSE(date1 < date2);
}

TEST(SQLiteDateTime_OperatorCompareGreaterThan, OneSecondDiff_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    ASSERT_GT(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareGreaterThan, SameDate_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_FALSE(date1 > date2);
}

TEST(SQLiteDateTime_OperatorCompareLessEqualThan, OneSecondDiff_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_LE(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareLessEqualThan, SameDate_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_TRUE(date1 <= date2);
}

TEST(SQLiteDateTime_OperatorCompareLessEqualThan, OneSecondDiffOver_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    ASSERT_FALSE(date1 <= date2);
}



TEST(SQLiteDateTime_OperatorCompareGreaterEqualThan, OneSecondDiff_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    ASSERT_GE(date1, date2);
}

TEST(SQLiteDateTime_OperatorCompareGreaterEqualThan, SameDate_ReturnTrue)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_TRUE(date1 >= date2);
}

TEST(SQLiteDateTime_OperatorCompareGreaterEqualThan, OneSecondDiffOver_ReturnFalse)
{
    auto date1 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,58)));
    auto date2 = SQLiteDateTimeFactory::NewDateTimeFromPTime(ptime(date(2019,12,31), time_duration(23,59,59)));
    ASSERT_FALSE(date1 >= date2);
}
