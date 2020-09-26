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
		}) 
	{
		assessments.begin()->addResult(AssessmentResult(Student(1, "Joe", "Blow"), 78.5f));
		assessments.begin()->addResult(AssessmentResult(Student(2, "Jane", "Doe"), 65.2f));
	}
    std::list<Assessment> getAllItems() override { return assessments;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Assessment &assessment) override { return insertResult; }
    bool updateItem(const Assessment &assessment) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<Assessment> assessments;
};

class AssessmentControllerTest : public ::testing::Test
{
public:
	AssessmentControllerTest() 
		: fakeStorage { make_unique<FakeAssessmentStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<AssessmentController>(DatabaseConnection("nulldb"), 
												 std::move(fakeStorage));
	}

	unique_ptr<IManagementItemStorage<Assessment>> fakeStorage;								 
	unique_ptr<AssessmentController> controller;
};

TEST(AssessmentController_Constructor, ValidArguments_ReturnSuccess)
{
	AssessmentController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<Assessment>>(make_unique<FakeAssessmentStorage>()));
}

TEST_F(AssessmentControllerTest, getAssessments_Return2Assessments)
{
	this->prepareController();
	controller->loadAssessments();
	ASSERT_EQ(2, controller->getAssessments().size());
	size_t index {0};
	for(const auto &assessment : controller->getAssessments()) {
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
	this->prepareController();
	controller->loadAssessments();
	ASSERT_EQ(nullptr, controller->findAssessment(0));
}

TEST_F(AssessmentControllerTest, findAssessment_WithIdOne_ReturnExam) 
{
	this->prepareController();
	controller->loadAssessments();
	auto actual = controller->findAssessment(1);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Exam of Aug 23 2020", actual->getName());
}

TEST_F(AssessmentControllerTest, findAssessment_WithIdTwo_ReturnExercice) 
{
	this->prepareController();
	controller->loadAssessments();
	auto actual = controller->findAssessment(2);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Exercice One 2020", actual->getName());
}

TEST_F(AssessmentControllerTest, insertAssessment_WithAssessmentThatWillSucceed_ReturnTrue) 
{
	this->prepareController();
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	ASSERT_TRUE(controller->insertAssessment(Assessment(1, "Exam of Aug 31 2020", 
									TestType("Exam"),
									Subject("French"),
									Class("MyClass", School("Test", City("CityTest"))),
									ptime(date(2020, Aug, 31), time_duration(8, 41, 32)))));
}

TEST_F(AssessmentControllerTest, insertAssessment_WithAssessmentThatWillFail_ReturnFailed) 
{
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	fakeAssessmentStorage->insertResult = false;
	fakeAssessmentStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertAssessment(Assessment(1, "Exam of Aug 31 2020", 
									TestType("Exam"),
									Subject("French"),
									Class("MyClass", School("Test", City("CityTest"))),
									ptime(date(2020, Aug, 31), time_duration(8, 41, 32)))));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(AssessmentControllerTest, updateAssessment_WithAssessmentThatWillSucceed_ReturnTrue) 
{
	this->prepareController();
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	ASSERT_TRUE(controller->updateAssessment(Assessment(1, "Exam of Aug 31 2020", 
									TestType("Exam"),
									Subject("French"),
									Class("MyClass", School("Test", City("CityTest"))),
									ptime(date(2020, Aug, 31), time_duration(8, 41, 32)))));
}

TEST_F(AssessmentControllerTest, updateAssessment_WithAssessmentThatWillFail_ReturnFailed) 
{
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	fakeAssessmentStorage->updateResult = false;
	fakeAssessmentStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateAssessment(Assessment(1, "Exam of Aug 31 2020", 
									TestType("Exam"),
									Subject("French"),
									Class("MyClass", School("Test", City("CityTest"))),
									ptime(date(2020, Aug, 31), time_duration(8, 41, 32)))));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(AssessmentControllerTest, deleteAssessment_WithAssessmentThatWillSucceed_ReturnTrue) 
{
	this->prepareController();
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	ASSERT_TRUE(controller->deleteAssessment(1));
}

TEST_F(AssessmentControllerTest, deleteAssessment_WithAssessmentThatWillFailFromConstraintError_ReturnFalse) 
{
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	fakeAssessmentStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteAssessment(1));
	ASSERT_EQ("Unable to delete the assessment because it is used by another item.", controller->getLastError());
}

TEST_F(AssessmentControllerTest, deleteAssessment_WithAssessmentThatWillFailFromGenericError_ReturnFalse) 
{
	auto fakeAssessmentStorage = dynamic_cast<FakeAssessmentStorage*>(this->fakeStorage.get());
	fakeAssessmentStorage->deleteResult = QueryResult::ERROR;
	fakeAssessmentStorage->lastError = "An error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteAssessment(1));
	ASSERT_EQ("An error occurred", controller->getLastError());
}