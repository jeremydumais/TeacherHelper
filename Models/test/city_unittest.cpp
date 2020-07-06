#include "city.h"
#include <gtest/gtest.h>

using namespace std;

TEST(City_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		City city("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(City_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		City city("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(City_Constructor, ValidEntry_ReturnSuccess)
{
	City city("New York");
	ASSERT_EQ("New York", city.getName());
}

TEST(City_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		City city(1, "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(City_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		City city(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(City_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	City city(1, "New York");
	ASSERT_EQ(1, city.getId());
	ASSERT_EQ("New York", city.getName());
}