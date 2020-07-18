#include "class.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Class_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Class Class("", School("School1", City("CityTest")));
		FAIL();	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Class Class("    ", School("School1", City("CityTest")));
		FAIL();	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Class Class("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Class_Constructor, ValidEntry_ReturnSuccess)
{
	Class Class("MyClass", School("Test", City("CityTest")));
	ASSERT_EQ("MyClass", Class.getName());
	ASSERT_EQ("Test", Class.getSchool().getName());
	ASSERT_EQ("CityTest", Class.getSchool().getCity().getName());
}

TEST(Class_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Class Class(1, "", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_ConstructorWithId, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Class Class(1, "   ", School("Test", City("CityTest")));
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
		Class Class(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Class_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Class Class(1, "MyClass", School("Test", City("CityTest")));
	ASSERT_EQ(1, Class.getId());
	ASSERT_EQ("MyClass", Class.getName());
	ASSERT_EQ("Test", Class.getSchool().getName());
	ASSERT_EQ("CityTest", Class.getSchool().getCity().getName());
}

TEST(Class_getId, WithId1_Return1)
{
	Class Class(1, "MyClass", School("Test", City("CityTest")));
	ASSERT_EQ(1, Class.getId());
}

TEST(Class_getName, WithNameMyClass_ReturnMyClass)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	ASSERT_EQ("MyClass", myClass.getName());
}

TEST(Class_setName, WithNameClassUpdated_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	myClass.setName("ClassUpdated");
	ASSERT_EQ("ClassUpdated", myClass.getName());
}

TEST(Class_setName, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Class myClass(1, "MyClass", School("Test", City("New York")));
		myClass.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_setName, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Class myClass(1, "MyClass", School("Test", City("New York")));
		myClass.setName("   ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Class_setName, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Class myClass(1, "MyClass", School("Test", City("New York")));
		myClass.setName("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Class_setSchool, WithSchoolTest1_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	myClass.setSchool(School("Test1", City("New York")));
	ASSERT_EQ("Test1", myClass.getSchool().getName());
}