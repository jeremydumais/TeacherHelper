#include "testTypeController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeTestTypeStorage : public IManagementItemStorage<TestType>
{
public:
    FakeTestTypeStorage()
		: testTypes(std::list<TestType> {
			TestType(1, "Exam"),
			TestType(2, "Exercice")
		}) {}
    std::list<TestType> getAllItems() override { return testTypes;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const TestType &testType) override { return true; }
    bool updateItem(const TestType &testType) override { return true; }
    bool deleteItem(size_t id) override { return true; }
private:
	std::list<TestType> testTypes;
    std::string lastError;
};

class TestTypeControllerTest : public ::testing::Test
{
public:
	TestTypeControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<TestType>>(make_unique<FakeTestTypeStorage>()))
	{}
	TestTypeController controller;
};

TEST(TestTypeController_Constructor, ValidArguments_ReturnSuccess)
{
	TestTypeController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<TestType>>(make_unique<FakeTestTypeStorage>()));
}

TEST_F(TestTypeControllerTest, getTestTypes_Return2TestTypes)
{
	controller.loadTestTypes();
	ASSERT_EQ(2, controller.getTestTypes().size());
	size_t index {0};
	for(const auto &testType : controller.getTestTypes()) {
		if (index == 0) {
			ASSERT_EQ("Exam", testType.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Exercice", testType.getName());
		}
		index++;
	}
}

TEST_F(TestTypeControllerTest, findTestType_WithIdZero_ReturnNullPtr) 
{
	controller.loadTestTypes();
	ASSERT_EQ(nullptr, controller.findTestType(0));
}

TEST_F(TestTypeControllerTest, findTestType_WithIdOne_ReturnExam) 
{
	controller.loadTestTypes();
	ASSERT_NE(nullptr, controller.findTestType(1));
	ASSERT_EQ("Exam", controller.findTestType(1)->getName());
}

TEST_F(TestTypeControllerTest, findTestType_WithIdTwo_ReturnExercice) 
{
	controller.loadTestTypes();
	ASSERT_NE(nullptr, controller.findTestType(2));
	ASSERT_EQ("Exercice", controller.findTestType(2)->getName());
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	controller.loadTestTypes();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("Oral comprehension"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Exam"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("EXAm"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Exam"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Exam   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Exam   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd(""));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	controller.loadTestTypes();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Oral comprehension", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	controller.loadTestTypes();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Exam", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Exam", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("EXAm", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	controller.loadTestTypes();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("EXAm", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Exam", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	controller.loadTestTypes();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("   Exam", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Exam   ", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Exam   ", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	controller.loadTestTypes();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   ", 1));
}

