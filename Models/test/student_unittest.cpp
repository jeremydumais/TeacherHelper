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

TEST(Student_Constructor, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("Test", "Test1", 
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(Student_Constructor, ValidEntry_ReturnSuccess)
{
	Student student("Joe", "Blow");
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
	ASSERT_EQ("", student.getComments());
}

TEST(Student_Constructor, ValidEntryWithComments_ReturnSuccess)
{
	Student student("Joe", "Blow", "This is a comment");
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
	ASSERT_EQ("This is a comment", student.getComments());
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

TEST(Student_ConstructorWithId, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student(1, "Test", "Test1", 
		"12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}

TEST(Student_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Student student(1, "Joe", "Blow");
	ASSERT_EQ(1, student.getId());
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
	ASSERT_EQ("", student.getComments());
}

TEST(Student_ConstructorWithId, ValidEntryWithComments_ReturnSuccess)
{
	Student student(1, "Joe", "Blow", "This is a comment");
	ASSERT_EQ(1, student.getId());
	ASSERT_EQ("Joe", student.getFirstName());
	ASSERT_EQ("Blow", student.getLastName());
	ASSERT_EQ("This is a comment", student.getComments());
}

TEST(Student_getFirstName, WithFirstNameJoe_ReturnJoe)
{
	Student student("Joe", "Blow");
	ASSERT_EQ("Joe", student.getFirstName());
}

TEST(Student_getLastName, WithLastNameBlow_ReturnBlow)
{
	Student student("Joe", "Blow");
	ASSERT_EQ("Blow", student.getLastName());
}

TEST(Student_getComments, WithCommentsTest_ReturnTest)
{
	Student student("Joe", "Blow", "Test");
	ASSERT_EQ("Test", student.getComments());
}

TEST(Student_getFullName, WithJoeBlow_ReturnFullName)
{
	Student student("Joe", "Blow", "Test");
	ASSERT_EQ("Joe Blow", student.getFullName());
}

TEST(Student_setFirstName, WithFirstNameJoe_ReturnJoe)
{
	Student student("Joe", "Blow");
	student.setFirstName("Jane");
	ASSERT_EQ("Jane", student.getFirstName());
}

TEST(Student_setFirstName, EmptyFirstName_ThrowInvalidArgument)
{
	try
	{
		Student student("Joe", "Blow");
		student.setFirstName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName cannot be null or empty.", err.what());
	}
}

TEST(Student_setFirstName, FirstNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("Joe", "Blow");
		student.setFirstName("adsjfhdjkhfjksdlhjkfhkfjdshfds1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("firstName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_setLastName, WithLastNameBlow_ReturnBlow)
{
	Student student("Joe", "Blow");
	student.setLastName("Doe");
	ASSERT_EQ("Doe", student.getLastName());
}

TEST(Student_setLastName, EmptyLastName_ThrowInvalidArgument)
{
	try
	{
		Student student("Joe", "Blow");
		student.setLastName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName cannot be null or empty.", err.what());
	}
}

TEST(Student_setLastName, LastNameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("Joe", "Blow");
		student.setLastName("adsjfhdjkhfjksdlhjkfhkfjdshfds1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("lastName must not be larger then 30 chars.", err.what());
	}
}

TEST(Student_setComments, WithCommentsTest_ReturnTest)
{
	Student student("Joe", "Blow", "Test");
	student.setComments("Another test");
	ASSERT_EQ("Another test", student.getComments());
}

TEST(Student_setComments, CommentsGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Student student("Joe", "Blow");
		student.setComments("12fdhsjdkhf sdkjh fjkdshfdkjfdshf sdkjf hdkjdshf dkjfh kfjdshf dskjfh dsjkf hdkjdjk dshfjkds hfdjskf hdsjkf hjksd hjkdsh djk sdkjfh kjfh fjkdslf dskjfksld; jfsdlkfj sdklfj dsklfj dsklfsdj fkldsj fdsklfj sdklfj sdklfjs dklfjs klfsdj fklsdj fsdffjdklfdjsffffj");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("comments must not be larger then 256 chars.", err.what());
	}
}