#include "assessmentController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

class FakeAssessmentStorage : public IManagementItemStorage<Assessment>
{
public:
    FakeAssessmentStorage()
		: assessments(std::list<Assessment> {
			Assessment(1, "Exam of Aug 23 2020", 
				  TestType("Exam"),
				  Subject("History"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33))),
			Assessment(2, "Exercice One 2020", 
				  TestType("Exercice"),
				  Subject("Math"),
				  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 21), time_duration(10, 14, 22)))
		}) {}
    std::list<Assessment> getAllItems() override { return assessments;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Assessment &assessment) override { return true; }
    bool updateItem(const Assessment &assessment) override { return true; }
    QueryResult deleteItem(size_t id) override { return QueryResult::OK; }
private:
	std::list<Assessment> assessments;
    std::string lastError;
};

class AssessmentControllerTest : public ::testing::Test
{
public:
	AssessmentControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<Assessment>>(make_unique<FakeAssessmentStorage>()))
	{}
	AssessmentController controller;
};

TEST(AssessmentController_Constructor, ValidArguments_ReturnSuccess)
{
	AssessmentController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<Assessment>>(make_unique<FakeAssessmentStorage>()));
}

TEST_F(AssessmentControllerTest, getAssessments_Return2Assessments)
{
	controller.loadAssessments();
	ASSERT_EQ(2, controller.getAssessments().size());
	size_t index {0};
	for(const auto &assessment : controller.getAssessments()) {
		if (index == 0) {
			ASSERT_EQ("Exam of Aug 23 2020", assessment.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Exercice One 2020", assessment.getName());
		}
		index++;
	}
}

TEST_F(AssessmentControllerTest, findAssessment_WithIdZero_ReturnNullPtr) 
{
	controller.loadAssessments();
	ASSERT_EQ(nullptr, controller.findAssessment(0));
}

TEST_F(AssessmentControllerTest, findAssessment_WithIdOne_ReturnExam) 
{
	controller.loadAssessments();
	ASSERT_NE(nullptr, controller.findAssessment(1));
	ASSERT_EQ("Exam of Aug 23 2020", controller.findAssessment(1)->getName());
}

TEST_F(AssessmentControllerTest, findAssessment_WithIdTwo_ReturnExercice) 
{
	controller.loadAssessments();
	ASSERT_NE(nullptr, controller.findAssessment(2));
	ASSERT_EQ("Exercice One 2020", controller.findAssessment(2)->getName());
}
