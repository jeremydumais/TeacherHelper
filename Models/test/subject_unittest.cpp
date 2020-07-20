#include "subject.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Subject_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Subject subject("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Subject_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Subject subject("    ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Subject_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Subject subject("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Subject_Constructor, ValidEntry_ReturnSuccess)
{
	Subject subject("Math");
	ASSERT_EQ("Math", subject.getName());
	ASSERT_EQ(false, subject.getIsDefault());
}

TEST(Subject_ConstructorWithIsDefault, ValidEntry_ReturnSuccess)
{
	Subject subject("Math", true);
	ASSERT_EQ("Math", subject.getName());
	ASSERT_EQ(true, subject.getIsDefault());
}

TEST(Subject_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Subject subject(1, "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Subject_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Subject subject(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Subject_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Subject subject(1, "Math");
	ASSERT_EQ(1, subject.getId());
	ASSERT_EQ("Math", subject.getName());
	ASSERT_EQ(false, subject.getIsDefault());
}

TEST(Subject_ConstructorWithIdWithDefault, ValidEntry_ReturnSuccess)
{
	Subject subject(1, "Math", true);
	ASSERT_EQ(1, subject.getId());
	ASSERT_EQ("Math", subject.getName());
	ASSERT_EQ(true, subject.getIsDefault());
}

TEST(Subject_getId, SampleMath1_Return1)
{
	Subject subject(1, "Math");
	ASSERT_EQ(1, subject.getId());
}

TEST(Subject_getName, SampleMath_ReturnMath)
{
	Subject subject("Math");
	ASSERT_EQ("Math", subject.getName());
}

TEST(Subject_getIsDefault, SampleMath_ReturnMath)
{
	Subject subject("Math", true);
	ASSERT_EQ(true, subject.getIsDefault());
}

TEST(Subject_setName, WithEnglish_ReturnSuccess)
{
	Subject subject("Math");
	subject.setName("English");
	ASSERT_EQ("English", subject.getName());
}

TEST(Subject_setName, WithEmptyName_ThrowInvalidArgument)
{
	try
	{
		Subject subject("Math");
		subject.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Subject_setName, WithWhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Subject subject("Math");
		subject.setName("    ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Subject_setName, WithNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Subject subject("Math");
		subject.setName("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Subject_setIsDefault, WithTrue_ReturnSuccess)
{
	Subject subject("Math");
	subject.setIsDefault(true);
	ASSERT_EQ(true, subject.getIsDefault());
}
