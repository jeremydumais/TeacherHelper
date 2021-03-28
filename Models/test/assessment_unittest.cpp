#include "assessment.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

class AssessmentSampleWithoutResults : public ::testing::Test
{
public:
	AssessmentSampleWithoutResults()
	  : assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33))) {}
	Assessment assessment;
};

class AssessmentSampleWithTwoResults : public ::testing::Test
{
public:
	AssessmentSampleWithTwoResults()
	  : assessment(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33)),
			  70.0f),
		result1 { Student(1, "Joe", "Blow"), 90.5f },
		result2 { Student(2, "Jane", "Doe"), 87.2f }
	{
		assessment.addResult(result1);
		assessment.addResult(result2);
	}
	Assessment assessment;
	AssessmentResult result1;
	AssessmentResult result2;
};

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
			  dateExpected,
			  70.0f);
	ASSERT_EQ(1, assessment.getId());  
	ASSERT_EQ("Intra Exam", assessment.getName());  
	ASSERT_EQ("Exam", assessment.getTestType().getName());
	ASSERT_EQ("History", assessment.getSubject().getName());
	ASSERT_EQ("MyClass", assessment.getClass().getName());
	ASSERT_EQ("Test", assessment.getClass().getSchool().getName());
	ASSERT_EQ("CityTest", assessment.getClass().getSchool().getCity().getName());
	ASSERT_EQ(dateExpected, assessment.getDateTime());
	ASSERT_EQ(70.0f, assessment.getMaxScore());
}

TEST_F(AssessmentSampleWithoutResults, getId_WithId1_Return1)
{
	ASSERT_EQ(1, assessment.getId());
}

TEST_F(AssessmentSampleWithoutResults, getName_WithNameIntraExam_ReturnMyTest)
{
	ASSERT_EQ("Intra Exam", assessment.getName());
}

TEST_F(AssessmentSampleWithoutResults, getTestType_WithTestTypeExam_ReturnExam)
{
	ASSERT_EQ("Exam", assessment.getTestType().getName());
}

TEST_F(AssessmentSampleWithoutResults, getSubject_WithSubjectHistory_ReturnHistory)
{
	ASSERT_EQ("History", assessment.getSubject().getName());
}

TEST_F(AssessmentSampleWithoutResults, getClass_WithClassMyClass_ReturnMyClass)
{
	ASSERT_EQ("MyClass", assessment.getClass().getName());
}

TEST_F(AssessmentSampleWithoutResults, getDate_With2020Aug24190405_ReturnSameDate)
{
	auto dateExpected { ptime(date(2020, Aug, 23), time_duration(13, 21, 33)) };
	ASSERT_EQ(dateExpected, assessment.getDateTime());
}

TEST_F(AssessmentSampleWithoutResults, getMaxScore_With100_Return100)
{
	ASSERT_EQ(100.0f, assessment.getMaxScore());
}

TEST_F(AssessmentSampleWithTwoResults, getMaxScore_With70_Return70)
{
	ASSERT_EQ(70.0f, assessment.getMaxScore());
}

TEST_F(AssessmentSampleWithoutResults, getResults_WithNoResults_ReturnEmptyVector)
{
	ASSERT_EQ(0, assessment.getResults().size());
}

TEST_F(AssessmentSampleWithTwoResults, getResults_WithTwoResults_ReturnVectorWith2Results)
{
	auto results = assessment.getResults();
	ASSERT_EQ(2, results.size());
	ASSERT_EQ("Joe", results[0].getStudent().getFirstName());
	ASSERT_EQ("Blow", results[0].getStudent().getLastName());
	ASSERT_EQ(90.5f, results[0].getResult());
	ASSERT_EQ("Jane", results[1].getStudent().getFirstName());
	ASSERT_EQ("Doe", results[1].getStudent().getLastName());
	ASSERT_EQ(87.2f, results[1].getResult());
}

TEST_F(AssessmentSampleWithoutResults, getStudentResult_WithInexistantStudent_ReturnNone)
{
	Student student(1, "Joe", "Blow");
	ASSERT_EQ(boost::none, assessment.getStudentResult(student));
}

TEST_F(AssessmentSampleWithTwoResults, getStudentResult_WithInexistantStudent_ReturnNone)
{
	Student student(5, "Santa", "Claus");
	ASSERT_EQ(boost::none, assessment.getStudentResult(student));
}

TEST_F(AssessmentSampleWithTwoResults, getStudentResult_WithExistingStudent_ReturnValidResult)
{
	auto results = assessment.getResults();
	Student student(1, "Joe", "Blow");
	auto actual { assessment.getStudentResult(student) };
	ASSERT_EQ(results[0].getResult(), actual->getResult());
	ASSERT_EQ(results[0].getStudent(), actual->getStudent());
	ASSERT_EQ(results[0].getId(), actual->getId());
}

TEST_F(AssessmentSampleWithoutResults, setName_WithNameTestUpdated_ReturnSuccess)
{
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

TEST_F(AssessmentSampleWithoutResults, setTestType_WithTestTypeExercice_ReturnSuccess)
{
	assessment.setTestType(TestType("Exercice"));
	ASSERT_EQ("Exercice", assessment.getTestType().getName());
}

TEST_F(AssessmentSampleWithoutResults, setSubject_WithSubjectMath_ReturnSuccess)
{
	assessment.setSubject(Subject("Math"));
	ASSERT_EQ("Math", assessment.getSubject().getName());
}

TEST_F(AssessmentSampleWithoutResults, setClass_WithClassAnotherClass_ReturnSuccess)
{
	assessment.setClass(Class("AnotherClass", School("Test", City("CityTest"))));
	ASSERT_EQ("AnotherClass", assessment.getClass().getName());
}

TEST_F(AssessmentSampleWithoutResults, setDate_WithClassAnotherClass_ReturnSuccess)
{
	auto expected { ptime(date(2019, Jan, 13), time_duration(3, 1, 3)) };
	assessment.setDate(expected);
	ASSERT_EQ(expected, assessment.getDateTime());
}

TEST_F(AssessmentSampleWithoutResults, setMaxScore_WithZero_ThrowInvalidArgument)
{
	try 
	{
		assessment.setMaxScore(0);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("The max score cannot be less than 1.", err.what());
	}
}

TEST_F(AssessmentSampleWithoutResults, setMaxScore_WithMinusOne_ThrowInvalidArgument)
{
	try 
	{
		assessment.setMaxScore(-1);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("The max score cannot be less than 1.", err.what());
	}
}

TEST_F(AssessmentSampleWithoutResults, setMaxScore_With32769_ThrowInvalidArgument)
{
	try 
	{
		assessment.setMaxScore(32769);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("The max score cannot be higher than max (32768).", err.what());
	}
}

TEST_F(AssessmentSampleWithoutResults, setMaxScore_With80_ReturnSuccess)
{
	assessment.setMaxScore(80);
	ASSERT_EQ(80.0f, assessment.getMaxScore());
}

TEST_F(AssessmentSampleWithoutResults, addResult_WithUniqueResult_ReturnSuccess)
{
	assessment.addResult(AssessmentResult(Student(1, "Joe", "Blow"), 90.5f));
	auto results = assessment.getResults();
	ASSERT_EQ(1, results.size());
	ASSERT_EQ("Joe", results[0].getStudent().getFirstName());
	ASSERT_EQ("Blow", results[0].getStudent().getLastName());
	ASSERT_EQ(90.5f, results[0].getResult());
}

TEST_F(AssessmentSampleWithoutResults, addResult_WithTwiceTheSameResult_ThrowInvalidArgument)
{
	Student student { 1, "Joe", "Blow" };
	try 
	{
		assessment.addResult(AssessmentResult(1, student, 90.0f));
		assessment.addResult(AssessmentResult(1, student, 90.0f));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("Cannot add the same assessment result twice.", err.what());
	}
}

TEST_F(AssessmentSampleWithoutResults, addResult_WithTwiceTheSameStudent_ThrowInvalidArgument)
{
	Student student { 1, "Joe", "Blow" };
	try 
	{
		assessment.addResult(AssessmentResult(1, student, 90.0f));
		assessment.addResult(AssessmentResult(student, 90.0f));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("Cannot add the same assessment result twice.", err.what());
	}
}

TEST_F(AssessmentSampleWithTwoResults, addResult_WithTwoDifferentStudents_ReturnSuccess)
{
	auto results = assessment.getResults();
	ASSERT_EQ(2, results.size());
	ASSERT_EQ("Joe", results[0].getStudent().getFirstName());
	ASSERT_EQ("Blow", results[0].getStudent().getLastName());
	ASSERT_EQ(90.5f, results[0].getResult());
	ASSERT_EQ("Jane", results[1].getStudent().getFirstName());
	ASSERT_EQ("Doe", results[1].getStudent().getLastName());
	ASSERT_EQ(87.2f, results[1].getResult());
}

TEST_F(AssessmentSampleWithTwoResults, removeMember_WithRemovingOneExistingResult_ReturnSuccess)
{
	auto results = assessment.getResults();
	ASSERT_EQ(2, assessment.getResults().size());

	assessment.removeResult(result1);

	results = assessment.getResults();
	ASSERT_EQ(1, assessment.getResults().size());
	ASSERT_EQ("Jane", results[0].getStudent().getFirstName());
	ASSERT_EQ("Doe", results[0].getStudent().getLastName());
	ASSERT_EQ(87.2f, results[0].getResult());
}

TEST_F(AssessmentSampleWithTwoResults, removeMember_WithRemovingOneNonExistingResult_ThrowInvalidArgument)
{
	ASSERT_EQ(2, assessment.getResults().size());

	try 
	{
		assessment.removeResult(AssessmentResult(3, Student(3, "Santa", "Claus"), 55.0f, ""));
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("That result is not part of the list.", err.what());
		ASSERT_EQ(2, assessment.getResults().size());
	}
}

TEST_F(AssessmentSampleWithTwoResults, clearResult_ReturnSuccess)
{
	ASSERT_EQ(2, assessment.getResults().size());
	assessment.clearResults();
	ASSERT_EQ(0, assessment.getResults().size());
}
