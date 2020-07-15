#include "school.h"
#include <gtest/gtest.h>

using namespace std;

TEST(School_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		School school("", City("Test"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(School_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		School school("   ", City("Test"));
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
		School school("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", City("Test"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(School_Constructor, ValidEntry_ReturnSuccess)
{
	School school("MySchool", City("New York"));
	ASSERT_EQ("MySchool", school.getName());
	ASSERT_EQ("New York", school.getCity().getName());
}

TEST(School_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		School school(1, "", City("Test"));
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
		School school(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", City("Test"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(School_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	School school(1, "MySchool", City("New York"));
	ASSERT_EQ(1, school.getId());
	ASSERT_EQ("MySchool", school.getName());
	ASSERT_EQ("New York", school.getCity().getName());
}

TEST(School_getId, WithId1_Return1)
{
	School school(1, "MySchool", City("New York"));
	ASSERT_EQ(1, school.getId());
}

TEST(School_getName, WithNameMySchool_ReturnMySchool)
{
	School school(1, "MySchool", City("New York"));
	ASSERT_EQ("MySchool", school.getName());
}

TEST(School_setName, WithNameSchoolUpdated_ReturnSuccess)
{
	School school(1, "MySchool", City("New York"));
	school.setName("SchoolUpdated");
	ASSERT_EQ("SchoolUpdated", school.getName());
}

TEST(School_setName, EmptyName_ThrowInvalidArgument)
{
	try
	{
		School school("School1", City("Test"));
		school.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(School_setName, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		School school("School1", City("Test"));
		school.setName("   ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(School_setName, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		School school("School1", City("Test"));
		school.setName("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(School_setCity, WithCityChicago_ReturnSuccess)
{
	School school(1, "MySchool", City("New York"));
	school.setCity(City("Chicago"));
	ASSERT_EQ("Chicago", school.getCity().getName());
}