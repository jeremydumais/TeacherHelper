#include "testResult.h"
#include <gtest/gtest.h>

using namespace std;

TEST(TestResult_Constructor, NegativeResultNearZero_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(Student("Joe", "Blow"), -0.01);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(TestResult_Constructor, NegativeResult_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(Student("Joe", "Blow"), -10);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(TestResult_Constructor, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(Student("Joe", "Blow"), 90,
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(TestResult_Constructor, ValidEntryWithoutComments_ReturnSuccess)
{
	TestResult testResult(Student("Joe", "Blow"), 90);
	ASSERT_EQ("Joe", testResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", testResult.getStudent().getLastName());
	ASSERT_EQ(90, testResult.getResult());
	ASSERT_TRUE(testResult.getComments().empty());
}

TEST(TestResult_Constructor, ValidEntryWithComments_ReturnSuccess)
{
	TestResult testResult(Student("Joe", "Blow"), 90, "This is a comment");
	ASSERT_EQ("Joe", testResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", testResult.getStudent().getLastName());
	ASSERT_EQ(90, testResult.getResult());
	ASSERT_EQ("This is a comment", testResult.getComments());
}

TEST(TestResultWithId_Constructor, NegativeResultNearZero_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(1, Student("Joe", "Blow"), -0.01);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(TestResultWithId_Constructor, NegativeResult_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(1, Student("Joe", "Blow"), -10);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(TestResultWithId_Constructor, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		TestResult testResult(1, Student("Joe", "Blow"), 90,
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(TestResultWithId_Constructor, ValidEntryWithoutComments_ReturnSuccess)
{
	TestResult testResult(1, Student("Joe", "Blow"), 90);
	ASSERT_EQ(1, testResult.getId());
	ASSERT_EQ("Joe", testResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", testResult.getStudent().getLastName());
	ASSERT_EQ(90, testResult.getResult());
	ASSERT_TRUE(testResult.getComments().empty());
}

TEST(TestResultWithId_Constructor, ValidEntryWithComments_ReturnSuccess)
{
	TestResult testResult(1, Student("Joe", "Blow"), 90, "This is a comment");
	ASSERT_EQ(1, testResult.getId());
	ASSERT_EQ("Joe", testResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", testResult.getStudent().getLastName());
	ASSERT_EQ(90, testResult.getResult());
	ASSERT_EQ("This is a comment", testResult.getComments());
}


TEST(TestResult_getId, SampleJoeBlow1_Return1)
{
	TestResult testResult(1, Student("Joe", "Blow"), 90);
	ASSERT_EQ(1, testResult.getId());
}

TEST(TestResult_getStudent, SampleJoeBlow_ReturnJoeBlow)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90);
	ASSERT_EQ(3, testResult.getStudent().getId());
	ASSERT_EQ("Joe", testResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", testResult.getStudent().getLastName());
}

TEST(TestResult_getResult, SampleJoeBlowResult90_Return90)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90);
	ASSERT_EQ(90, testResult.getResult());
}

TEST(TestResult_getComments, SampleJoeBlow_ReturnTheComments)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	ASSERT_EQ("A comment", testResult.getComments());
}


TEST(TestResult_setStudent, WithJaneDow_ReturnSuccess)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	testResult.setStudent(Student(4, "Jane", "Doe"));
	ASSERT_EQ(4, testResult.getStudent().getId());
	ASSERT_EQ("Jane", testResult.getStudent().getFirstName());
	ASSERT_EQ("Doe", testResult.getStudent().getLastName());
}

TEST(TestResult_setResult, With80_ReturnSuccess)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	testResult.setResult(80);
	ASSERT_EQ(80, testResult.getResult());
}

TEST(TestResult_setComments, WithNewComment_ReturnSuccess)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	testResult.setComments("A New Comment!!!");
	ASSERT_EQ("A New Comment!!!", testResult.getComments());
}

TEST(TestResult_setComments, WithEmptyComment_ReturnSuccess)
{
	TestResult testResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	testResult.setComments("");
	ASSERT_TRUE(testResult.getComments().empty());
}
