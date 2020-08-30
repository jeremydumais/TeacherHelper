#include "assessment.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

TEST(Assessment_Constructor, EmptyName_ThrowInvalidArgument)
{
	try {
		Assessment assessment("", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();	
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_Constructor, WhiteSpacesName_ThrowInvalidArgument)
{
	try {
		Assessment assessment("    ", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();	
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_Constructor, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try {
		Assessment assessment("ddjsakldjasklsajaklsjdsalksjksaljdsakdsjaslakjslksjskl;djsaldjsaljsadkldjskldsajkhksahdjskadhaskhdsd7", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name must not be larger then 100 chars.", err.what());
	}
}


TEST(Assessment_Constructor, ValidEntry_ReturnSuccess)
{
	auto dateExpected { ptime(date(2020, Aug, 23), time_duration(13, 21, 33)) };
	Assessment assessment("Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
			  dateExpected);
	ASSERT_EQ("Intra Exam", assessment.getName());  
	ASSERT_EQ("Exam", assessment.getTestType().getName());
	ASSERT_EQ("History", assessment.getSubject().getName());
	ASSERT_EQ("MyClass", assessment.getClass().getName());
	ASSERT_EQ("Test", assessment.getClass().getSchool().getName());
	ASSERT_EQ("CityTest", assessment.getClass().getSchool().getCity().getName());
	ASSERT_EQ(dateExpected, assessment.getDateTime());
}

TEST(Assessment_ConstructorWithId, EmptyName_ThrowInvalidArgument)
{
	try {
		Assessment assessment(1, "", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();	
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_ConstructorWithId, WhiteSpacesName_ThrowInvalidArgument)
{
	try {
		Assessment assessment(1, "    ", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();	
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_ConstructorWithId, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try {
		Assessment assessment(1, "ddjsakldjasklsajaklsjdsalksjksaljdsakdsjaslakjslksjskl;djsaldjsaljsadkldjskldsajkhksahdjskadhaskhdsd7", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		FAIL();
	}
	catch(invalid_argument &err) {
        ASSERT_STREQ("name must not be larger then 100 chars.", err.what());
	}
}

TEST(Assessment_ConstructorWithId, ValidEntry_ReturnSuccess)
{
	auto dateExpected { ptime(date(2020, Aug, 23), time_duration(13, 21, 33)) };
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
			  dateExpected);
	ASSERT_EQ(1, assessment.getId());  
	ASSERT_EQ("Intra Exam", assessment.getName());  
	ASSERT_EQ("Exam", assessment.getTestType().getName());
	ASSERT_EQ("History", assessment.getSubject().getName());
	ASSERT_EQ("MyClass", assessment.getClass().getName());
	ASSERT_EQ("Test", assessment.getClass().getSchool().getName());
	ASSERT_EQ("CityTest", assessment.getClass().getSchool().getCity().getName());
	ASSERT_EQ(dateExpected, assessment.getDateTime());
}

TEST(Assessment_getId, WithId1_Return1)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ(1, assessment.getId());
}

TEST(Assessment_getName, WithNameIntraExam_ReturnMyTest)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ("Intra Exam", assessment.getName());
}

TEST(Assessment_getTestType, WithTestTypeExam_ReturnExam)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ("Exam", assessment.getTestType().getName());
}

TEST(Assessment_getSubject, WithSubjectHistory_ReturnHistory)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ("History", assessment.getSubject().getName());
}

TEST(Assessment_getClass, WithClassMyClass_ReturnMyClass)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ("MyClass", assessment.getClass().getName());
}

TEST(Assessment_getDate, With2020Aug24190405_ReturnSameDate)
{
	auto dateExpected { ptime(date(2020, Aug, 24), time_duration(19, 04, 05)) };
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
			  dateExpected);
	ASSERT_EQ(dateExpected, assessment.getDateTime());
}

/*TEST(Assessment_getResults, WithNoResults_ReturnEmptyVector)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	ASSERT_EQ(0, assessment.getResults().size());
}

TEST(Assessment_getResults, WithTwoResults_ReturnVectorWith2Results)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.addResult(AssessmentResult(Student(1, "Joe", "Blow"), 90.5f));
	assessment.addResult(AssessmentResult(Student(2, "Jane", "Doe"), 81.2f));
	auto results = assessment.getResults();
	ASSERT_EQ(2, results.size());
	ASSERT_EQ("Joe", results[0].getStudent().getFirstName());
	ASSERT_EQ("Blow", results[0].getStudent().getLastName());
	ASSERT_EQ(90.5f, results[0].getResult());
	ASSERT_EQ("Jane", results[1].getStudent().getFirstName());
	ASSERT_EQ("Doe", results[1].getStudent().getLastName());
	ASSERT_EQ(81.2f, results[1].getResult());
}*/

TEST(Assessment_setName, WithNameTestUpdated_ReturnSuccess)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.setName("Another Exam");
	ASSERT_EQ("Another Exam", assessment.getName());
}

TEST(Assessment_setName, EmptyName_ThrowInvalidArgument)
{
	try
	{
		Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		assessment.setName("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_setName, WhiteSpacesName_ThrowInvalidArgument)
{
	try
	{
		Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		assessment.setName("   ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name cannot be null or empty.", err.what());
	}
}

TEST(Assessment_setName, NameGreaterThenLimit_ThrowInvalidArgument)
{
	try
	{
		Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
		assessment.setName("ddjsakldjasklsajaklsjdsalksjksaljdsakdsjaslakjslksjskl;djsaldjsaljsadkldjskldsajkhksahdjskadhaskhdsd7");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("name must not be larger then 100 chars.", err.what());
	}
}

TEST(Assessment_setTestType, WithTestTypeExercice_ReturnSuccess)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.setTestType(TestType("Exercice"));
	ASSERT_EQ("Exercice", assessment.getTestType().getName());
}

TEST(Assessment_setSubject, WithSubjectMath_ReturnSuccess)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.setSubject(Subject("Math"));
	ASSERT_EQ("Math", assessment.getSubject().getName());
}

TEST(Assessment_setClass, WithClassAnotherClass_ReturnSuccess)
{
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.setClass(Class("AnotherClass", School("Test", City("CityTest"))));
	ASSERT_EQ("AnotherClass", assessment.getClass().getName());
}

TEST(Assessment_setDate, WithClassAnotherClass_ReturnSuccess)
{
	auto expected { ptime(date(2019, Jan, 13), time_duration(3, 1, 3)) };
	Assessment assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
			  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)));
	assessment.setDate(expected);
	ASSERT_EQ(expected, assessment.getDateTime());
}

/*
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