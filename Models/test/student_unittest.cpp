#include "student.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Student_Constructor, EmptyFirstName_ThrowInvalidArgument)
{
	try
	{
		Student student("", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName cannot be null or empty.", err.what());
	}
}

TEST(Student_Constructor, FirstNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("adsjfhdjkhfjksdlhjkfhkfjdshfds1", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_Constructor, EmptyLastName_ThrowInvalidArgument)
{
	try
	{
		Student student("Test", "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName cannot be null or empty.", err.what());
	}
}

TEST(Student_Constructor, LastNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("Test", "adsjfhdjkhfjksdlhjkfhkfjdshfds1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_Constructor, ValidEntry_ReturnSuccess)
{
	Student student("Joe", "Blow");
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
}

TEST(Student_ConstructorWithId, EmptyFirstName_ThrowInvalidArgument)
{
	try
	{
		Student student(1, "", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName cannot be null or empty.", err.what());
	}
}

TEST(Student_ConstructorWithId, FirstNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1", "Test");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_ConstructorWithId, EmptyLastName_ThrowInvalidArgument)
{
	try
	{
		Student student(1, "Test", "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName cannot be null or empty.", err.what());
	}
}

TEST(Student_ConstructorWithId, LastNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student(1, "Test", "adsjfhdjkhfjksdlhjkfhkfjdshfds1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Student student(1, "Joe", "Blow");
	ASSERT_EQ(1, student.getId());
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
}