#include "class.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Class_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Class Class("", School("Test", "CityTest"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Class Class("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", "CityTest"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Class_Constructor, ValidEntry_ReturnSuccess)
{
	Class Class("MyClass", School("Test", "CityTest"));
	ASSERT_EQ("MyClass", Class.getName());
	ASSERT_EQ("Test", Class.getSchool().getName());
	ASSERT_EQ("CityTest", Class.getSchool().getCity());
}

TEST(Class_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Class Class(1, "", School("Test", "CityTest"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Class Class(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", "CityTest"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Class_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Class Class(1, "MyClass", School("Test", "CityTest"));
	ASSERT_EQ(1, Class.getId());
	ASSERT_EQ("MyClass", Class.getName());
	ASSERT_EQ("Test", Class.getSchool().getName());
	ASSERT_EQ("CityTest", Class.getSchool().getCity());
}