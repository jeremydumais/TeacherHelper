#include "testType.h"
#include <gtest/gtest.h>

using namespace std;

TEST(TestType_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		TestType testType("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(TestType_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		TestType testType("    ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(TestType_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		TestType testType("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(TestType_Constructor, ValidEntry_ReturnSuccess)
{
	TestType testType("Exam");
	ASSERT_EQ("Exam", testType.getName());
}

TEST(TestType_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		TestType testType(1, "");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(TestType_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		TestType testType(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(TestType_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	TestType testType(1, "Exam");
	ASSERT_EQ(1, testType.getId());
	ASSERT_EQ("Exam", testType.getName());
}

TEST(TestType_getId, SampleExam1_Return1)
{
	TestType testType(1, "Exam");
	ASSERT_EQ(1, testType.getId());
}

TEST(TestType_getName, SampleExam_ReturnExam)
{
	TestType testType("Exam");
	ASSERT_EQ("Exam", testType.getName());
}

TEST(TestType_setName, WithExercice_ReturnSuccess)
{
	TestType testType("Exam");
	testType.setName("Exercice");
	ASSERT_EQ("Exercice", testType.getName());
}

TEST(TestType_setName, WithEmptyName_ThrowInvalidArgument)
{
	try
	{
		TestType testType("Exam");
		testType.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(TestType_setName, WithWhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		TestType testType("Exam");
		testType.setName("    ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(TestType_setName, WithNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		TestType testType("Exam");
		testType.setName("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

