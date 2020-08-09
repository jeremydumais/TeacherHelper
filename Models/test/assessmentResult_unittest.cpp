#include "assessmentResult.h"
#include <gtest/gtest.h>

using namespace std;

TEST(AssessmentResult_Constructor, NegativeResultNearZero_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(Student("Joe", "Blow"), -0.01);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(AssessmentResult_Constructor, NegativeResult_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(Student("Joe", "Blow"), -10);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(AssessmentResult_Constructor, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(Student("Joe", "Blow"), 90,
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(AssessmentResult_Constructor, ValidEntryWithoutComments_ReturnSuccess)
{
	AssessmentResult assessmentResult(Student("Joe", "Blow"), 90);
	ASSERT_EQ("Joe", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", assessmentResult.getStudent().getLastName());
	ASSERT_EQ(90, assessmentResult.getResult());
	ASSERT_TRUE(assessmentResult.getComments().empty());
}

TEST(AssessmentResult_Constructor, ValidEntryWithComments_ReturnSuccess)
{
	AssessmentResult assessmentResult(Student("Joe", "Blow"), 90, "This is a comment");
	ASSERT_EQ("Joe", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", assessmentResult.getStudent().getLastName());
	ASSERT_EQ(90, assessmentResult.getResult());
	ASSERT_EQ("This is a comment", assessmentResult.getComments());
}

TEST(AssessmentResultWithId_Constructor, NegativeResultNearZero_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(1, Student("Joe", "Blow"), -0.01);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(AssessmentResultWithId_Constructor, NegativeResult_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(1, Student("Joe", "Blow"), -10);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("result must be a positive number.", err.what());
	}
}

TEST(AssessmentResultWithId_Constructor, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		AssessmentResult assessmentResult(1, Student("Joe", "Blow"), 90,
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(AssessmentResultWithId_Constructor, ValidEntryWithoutComments_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student("Joe", "Blow"), 90);
	ASSERT_EQ(1, assessmentResult.getId());
	ASSERT_EQ("Joe", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", assessmentResult.getStudent().getLastName());
	ASSERT_EQ(90, assessmentResult.getResult());
	ASSERT_TRUE(assessmentResult.getComments().empty());
}

TEST(AssessmentResultWithId_Constructor, ValidEntryWithComments_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student("Joe", "Blow"), 90, "This is a comment");
	ASSERT_EQ(1, assessmentResult.getId());
	ASSERT_EQ("Joe", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", assessmentResult.getStudent().getLastName());
	ASSERT_EQ(90, assessmentResult.getResult());
	ASSERT_EQ("This is a comment", assessmentResult.getComments());
}


TEST(AssessmentResult_getId, SampleJoeBlow1_Return1)
{
	AssessmentResult assessmentResult(1, Student("Joe", "Blow"), 90);
	ASSERT_EQ(1, assessmentResult.getId());
}

TEST(AssessmentResult_getStudent, SampleJoeBlow_ReturnJoeBlow)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90);
	ASSERT_EQ(3, assessmentResult.getStudent().getId());
	ASSERT_EQ("Joe", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Blow", assessmentResult.getStudent().getLastName());
}

TEST(AssessmentResult_getResult, SampleJoeBlowResult90_Return90)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90);
	ASSERT_EQ(90, assessmentResult.getResult());
}

TEST(AssessmentResult_getComments, SampleJoeBlow_ReturnTheComments)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	ASSERT_EQ("A comment", assessmentResult.getComments());
}


TEST(AssessmentResult_setStudent, WithJaneDow_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	assessmentResult.setStudent(Student(4, "Jane", "Doe"));
	ASSERT_EQ(4, assessmentResult.getStudent().getId());
	ASSERT_EQ("Jane", assessmentResult.getStudent().getFirstName());
	ASSERT_EQ("Doe", assessmentResult.getStudent().getLastName());
}

TEST(AssessmentResult_setResult, With80_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	assessmentResult.setResult(80);
	ASSERT_EQ(80, assessmentResult.getResult());
}

TEST(AssessmentResult_setComments, WithNewComment_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	assessmentResult.setComments("A New Comment!!!");
	ASSERT_EQ("A New Comment!!!", assessmentResult.getComments());
}

TEST(AssessmentResult_setComments, WithEmptyComment_ReturnSuccess)
{
	AssessmentResult assessmentResult(1, Student(3, "Joe", "Blow"), 90, "A comment");
	assessmentResult.setComments("");
	ASSERT_TRUE(assessmentResult.getComments().empty());
}
