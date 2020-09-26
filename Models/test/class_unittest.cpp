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

TEST(Class_getMembers, WithNoMembers_ReturnEmptyList)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	ASSERT_EQ(0, myClass.getMembers().size());
}


TEST(Class_getMembers, WithTwoMembers_ReturnListWith2Members)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	myClass.addMember(Student(1, "Joe", "Blow"));
	myClass.addMember(Student(2, "Jane", "Doe"));
	ASSERT_EQ(2, myClass.getMembers().size());
	size_t index {0};
	for(const auto &student : myClass.getMembers()) {
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

TEST(Class_addMember, WithValidMember_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	Student expected { "Joe", "Blow" };
	myClass.addMember(expected);
	ASSERT_EQ(1, myClass.getMembers().size());
	const auto &actual { myClass.getMembers().begin() };
	ASSERT_EQ(expected.getFirstName(), actual->getFirstName());
	ASSERT_EQ(expected.getLastName(), actual->getLastName());
}

TEST(Class_addMember, WithTwiceTheSameMember_ThrowInvalidArgument)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	Student expected { 1, "Joe", "Blow" };
	myClass.addMember(expected);
	try 
	{
		myClass.addMember(expected);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("Cannot add the same member twice.", err.what());
	}
}

TEST(Class_addMember, WithTwoDifferentMembers_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	Student member1 { 1, "Joe", "Blow" };
	Student member2 { 2, "Joe", "Blow" };
	myClass.addMember(member1);
	myClass.addMember(member2);
	ASSERT_EQ(2, myClass.getMembers().size());
	size_t index {0};
	for(const auto &student : myClass.getMembers()) {
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

TEST(Class_removeMember, WithValidMember_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	Student expected { "Joe", "Blow" };
	myClass.addMember(expected);
	ASSERT_EQ(1, myClass.getMembers().size());
	myClass.removeMember(expected);
	ASSERT_EQ(0, myClass.getMembers().size());
}

TEST(Class_removeMember, WithStudentNotPartOfTheList_ThrowInvalidArgument)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	Student expected { 1, "Joe", "Blow" };
	myClass.addMember(expected);
	try 
	{
		myClass.removeMember(Student(2, "Jane", "Doe"));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("That student is not part of the list.", err.what());
	}
}

TEST(Class_clearMembers, With2Members_ReturnSuccess)
{
	Class myClass(1, "MyClass", School("Test", City("New York")));
	myClass.addMember(Student { 1, "Joe", "Blow" });
	myClass.addMember(Student { 1, "Jane", "Doe" });
	ASSERT_EQ(2, myClass.getMembers().size());
	myClass.clearMembers();
	ASSERT_EQ(0, myClass.getMembers().size());
}