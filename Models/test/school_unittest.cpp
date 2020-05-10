#include "school.h"
#include <gtest/gtest.h>

using namespace std;

TEST(School_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		School School("", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(School_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		School School("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(School_Constructor, EmptyCity_ThrowInvalidArgument)
{
	try
	{
		School School("Test", "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("city cannot be null or empty.", err.what());
	}
}

TEST(School_Constructor, CityGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		School School("Test", "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("city must not be larger then 50 chars.", err.what());
	}
}

TEST(School_Constructor, ValidEntry_ReturnSuccess)
{
	School School("MySchool", "New York");
	ASSERT_EQ("MySchool", School.getName());
	ASSERT_EQ("New York", School.getCity());
}

TEST(School_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		School School(1, "", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(School_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		School School(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(School_ConstructorWithId, EmptyCity_ThrowInvalidArgument)
{
	try
	{
		School School(1, "Test", "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("city cannot be null or empty.", err.what());
	}
}

TEST(School_ConstructorWithId, CityGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		School School(1, "Test", "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("city must not be larger then 50 chars.", err.what());
	}
}

TEST(School_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	School School(1, "MySchool", "New York");
	ASSERT_EQ(1, School.getId());
	ASSERT_EQ("MySchool", School.getName());
	ASSERT_EQ("New York", School.getCity());
}