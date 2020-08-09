#include "assessment.h"
#include <gtest/gtest.h>

using namespace std;

TEST(Assessment_Constructor, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Assessment assessment("", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))));
		FAIL();	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}
/*
TEST(Test_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Test Test("    ", School("School1", City("CityTest")));
		FAIL();	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Test_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Test Test("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Test_Constructor, ValidEntry_ReturnSuccess)
{
	Test Test("MyTest", School("Test", City("CityTest")));
	ASSERT_EQ("MyTest", Test.getName());
	ASSERT_EQ("Test", Test.getSchool().getName());
	ASSERT_EQ("CityTest", Test.getSchool().getCity().getName());
}

TEST(Test_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Test Test(1, "", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Test_ConstructorWithId, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Test Test(1, "   ", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Test_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Test Test(1, "adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442", School("Test", City("CityTest")));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Test_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	Test Test(1, "MyTest", School("Test", City("CityTest")));
	ASSERT_EQ(1, Test.getId());
	ASSERT_EQ("MyTest", Test.getName());
	ASSERT_EQ("Test", Test.getSchool().getName());
	ASSERT_EQ("CityTest", Test.getSchool().getCity().getName());
}

TEST(Test_getId, WithId1_Return1)
{
	Test Test(1, "MyTest", School("Test", City("CityTest")));
	ASSERT_EQ(1, Test.getId());
}

TEST(Test_getName, WithNameMyTest_ReturnMyTest)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	ASSERT_EQ("MyTest", myTest.getName());
}

TEST(Test_getMembers, WithNoMembers_ReturnEmptyList)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	ASSERT_EQ(0, myTest.getMembers().size());
}


TEST(Test_getMembers, WithTwoMembers_ReturnListWith2Members)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	myTest.addMember(Student(1, "Joe", "Blow"));
	myTest.addMember(Student(2, "Jane", "Doe"));
	ASSERT_EQ(2, myTest.getMembers().size());
	size_t index {0};
	for(const auto &student : myTest.getMembers()) {
		if (index == 0) {
			ASSERT_EQ("Joe", student.getFirstName());
			ASSERT_EQ("Blow", student.getLastName());
		}
		else if (index == 1) {
			ASSERT_EQ("Jane", student.getFirstName());
			ASSERT_EQ("Doe", student.getLastName());
			}
		index++;
	}
}

TEST(Test_setName, WithNameTestUpdated_ReturnSuccess)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	myTest.setName("TestUpdated");
	ASSERT_EQ("TestUpdated", myTest.getName());
}

TEST(Test_setName, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Test myTest(1, "MyTest", School("Test", City("New York")));
		myTest.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Test_setName, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Test myTest(1, "MyTest", School("Test", City("New York")));
		myTest.setName("   ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Test_setName, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Test myTest(1, "MyTest", School("Test", City("New York")));
		myTest.setName("adsjfhdjkhfjksdlhjkfhkfjdshfds1098 9044903808038442");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 50 chars.", err.what());
	}
}

TEST(Test_setSchool, WithSchoolTest1_ReturnSuccess)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	myTest.setSchool(School("Test1", City("New York")));
	ASSERT_EQ("Test1", myTest.getSchool().getName());
}

TEST(Test_addMember, WithValidMember_ReturnSuccess)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	Student expected { "Joe", "Blow" };
	myTest.addMember(expected);
	ASSERT_EQ(1, myTest.getMembers().size());
	const auto &actual { myTest.getMembers().begin() };
	ASSERT_EQ(expected.getFirstName(), actual->getFirstName());
	ASSERT_EQ(expected.getLastName(), actual->getLastName());
}

TEST(Test_addMember, WithTwiceTheSameMember_ThrowInvalidArgument)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	Student expected { 1, "Joe", "Blow" };
	myTest.addMember(expected);
	try 
	{
		myTest.addMember(expected);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("Cannot add the same member twice.", err.what());
	}
}

TEST(Test_addMember, WithTwoDifferentMembers_ReturnSuccess)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	Student member1 { 1, "Joe", "Blow" };
	Student member2 { 2, "Joe", "Blow" };
	myTest.addMember(member1);
	myTest.addMember(member2);
	ASSERT_EQ(2, myTest.getMembers().size());
	size_t index {0};
	for(const auto &student : myTest.getMembers()) {
		if (index == 0) {
			ASSERT_EQ("Joe", student.getFirstName());
			ASSERT_EQ("Blow", student.getLastName());
		}
		else if (index == 1) {
			ASSERT_EQ("Joe", student.getFirstName());
			ASSERT_EQ("Blow", student.getLastName());
			}
		index++;
	}


}

TEST(Test_removeMember, WithValidMember_ReturnSuccess)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	Student expected { "Joe", "Blow" };
	myTest.addMember(expected);
	ASSERT_EQ(1, myTest.getMembers().size());
	myTest.removeMember(expected);
	ASSERT_EQ(0, myTest.getMembers().size());
}

TEST(Test_removeMember, WithStudentNotPartOfTheList_ThrowInvalidArgument)
{
	Test myTest(1, "MyTest", School("Test", City("New York")));
	Student expected { 1, "Joe", "Blow" };
	myTest.addMember(expected);
	try 
	{
		myTest.removeMember(Student(2, "Jane", "Doe"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("That student is not part of the list.", err.what());
	}
}*/