#include "testTypeController.h"
#include "fakeDatabaseController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeTestTypeStorage : public ManagementItemStorageBase<TestType>
{
public:
    FakeTestTypeStorage()
		: testTypes(std::list<TestType> {
			TestType(1, "Exam"),
			TestType(2, "Exercice")
		}),
		ManagementItemStorageBase<TestType>(FakeDatabaseController().getDatabaseConnection()) {}
    std::list<TestType> getAllItems() override { return testTypes;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const TestType &testType) override { return insertResult; }
    bool updateItem(const TestType &testType) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool isReferentialIntegrityConstraint(size_t id) override { return true; };
	std::string getSelectCommand() const override { return ""; };
    TestType getItemFromRecord(const IStorageSelectOperation &record) const override { return { 1, "FakeTypeTest" }; };
    std::string getInsertCommand() const override { return ""; };
    std::vector<std::string> getInsertValues(const TestType &item) const override { return {"", ""}; };
    std::string getUpdateCommand() const override {return ""; };
    std::vector<std::string> getUpdateValues(const TestType &item) const override { return {"", ""}; };
    std::string getDeleteCommand() const override {return ""; };
    std::vector<std::string> getDeleteValues(size_t id) const override { return {""}; };
    std::string getReferentialIntegrityConstraintsCommand() const override { return ""; };
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override { return {""}; };
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<TestType> testTypes;
};

class TestTypeControllerTest : public ::testing::Test
{
public:
	TestTypeControllerTest() 
		: fakeStorage { make_unique<FakeTestTypeStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<TestTypeController>(FakeDatabaseController(), 
												 std::move(fakeStorage));
	}
	unique_ptr<ManagementItemStorageBase<TestType>> fakeStorage;								 
	unique_ptr<TestTypeController> controller;
};

TEST(TestTypeController_Constructor, ValidArguments_ReturnSuccess)
{
	TestTypeController controller(FakeDatabaseController(), make_unique<FakeTestTypeStorage>());
}

TEST_F(TestTypeControllerTest, getTestTypes_Return2TestTypes)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_EQ(2, controller->getTestTypes().size());
	size_t index {0};
	for(const auto &testType : controller->getTestTypes()) {
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
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_EQ(nullptr, controller->findTestType(0));
}

TEST_F(TestTypeControllerTest, findTestType_WithIdOne_ReturnExam) 
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_NE(nullptr, controller->findTestType(1));
	ASSERT_EQ("Exam", controller->findTestType(1)->getName());
}

TEST_F(TestTypeControllerTest, findTestType_WithIdTwo_ReturnExercice) 
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_NE(nullptr, controller->findTestType(2));
	ASSERT_EQ("Exercice", controller->findTestType(2)->getName());
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("Oral comprehension"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Exam"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("EXAm"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Exam"));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Exam   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Exam   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd(""));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   "));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Oral comprehension", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Exam", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Exam", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("EXAm", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("EXAm", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Exam", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("   Exam", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Exam   ", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Exam   ", 2));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("", 1));
}

TEST_F(TestTypeControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadTestTypes();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   ", 1));
}

TEST_F(TestTypeControllerTest, insertTestType_WithTestTypeThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertTestType(TestType { "Final Exam" }));
}

TEST_F(TestTypeControllerTest, insertTestType_WithTestTypeThatWillFailed_ReturnFailed) 
{
	auto fakeTestTypeStorage = dynamic_cast<FakeTestTypeStorage*>(this->fakeStorage.get());
	fakeTestTypeStorage->insertResult = false;
	fakeTestTypeStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertTestType(TestType { "Final Exam" }));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(TestTypeControllerTest, updateTestType_WithTestTypeThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateTestType(TestType { "Exam" }));
}

TEST_F(TestTypeControllerTest, updateTestType_WithTestTypeThatWillFailed_ReturnFailed) 
{
	auto fakeTestTypeStorage = dynamic_cast<FakeTestTypeStorage*>(this->fakeStorage.get());
	fakeTestTypeStorage->updateResult = false;
	fakeTestTypeStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateTestType(TestType { "Final Exam" }));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(TestTypeControllerTest, deleteTestType_WithTestTypeThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteTestType(1));
}

TEST_F(TestTypeControllerTest, deleteTestType_WithTestTypeThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeTestTypeStorage = dynamic_cast<FakeTestTypeStorage*>(this->fakeStorage.get());
	fakeTestTypeStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteTestType(1));
	ASSERT_EQ("Unable to delete the test type because it is used by another item. (Probably a test)", controller->getLastError());
}

TEST_F(TestTypeControllerTest, deleteTestType_WithTestTypeThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeTestTypeStorage = dynamic_cast<FakeTestTypeStorage*>(this->fakeStorage.get());
	fakeTestTypeStorage->deleteResult = QueryResult::ERROR;
	fakeTestTypeStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteTestType(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}