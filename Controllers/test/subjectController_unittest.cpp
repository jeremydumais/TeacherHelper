#include "subjectController.h"
#include "fakeDatabaseController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeSubjectStorage : public ManagementItemStorageBase<Subject>
{
public:
    FakeSubjectStorage()
		: subjects(std::list<Subject> {
			Subject(1, "Math", true),
			Subject(2, "English")
		}),
		ManagementItemStorageBase<Subject>(FakeDatabaseController().getDatabaseConnection()) {}
    std::list<Subject> getAllItems() override { return subjects;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Subject &subject) override { return insertResult; }
    bool updateItem(const Subject &subject) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
    bool isReferentialIntegrityConstraint(size_t id) override { return true; };
	std::string getSelectCommand() const override { return ""; };
    Subject getItemFromRecord(const IStorageSelectOperation &record) const override { return { 1, "FakeSubject" }; };
    std::string getInsertCommand() const override { return ""; };
    std::vector<std::string> getInsertValues(const Subject &item) const override { return {"", ""}; };
    std::string getUpdateCommand() const override {return ""; };
    std::vector<std::string> getUpdateValues(const Subject &item) const override { return {"", ""}; };
    std::string getDeleteCommand() const override {return ""; };
    std::vector<std::string> getDeleteValues(size_t id) const override { return {""}; };
    std::string getReferentialIntegrityConstraintsCommand() const override { return ""; };
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override { return {""}; };
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
	std::string lastError;
private:
	std::list<Subject> subjects;
};

class SubjectControllerTest : public ::testing::Test
{
public:
	SubjectControllerTest() 
		: fakeStorage { make_unique<FakeSubjectStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<SubjectController>(FakeDatabaseController(), 
												 std::move(fakeStorage));
	}
	unique_ptr<ManagementItemStorageBase<Subject>> fakeStorage;								 
	unique_ptr<SubjectController> controller;
};

TEST(SubjectController_Constructor, ValidArguments_ReturnSuccess)
{
	SubjectController controller(FakeDatabaseController(), make_unique<FakeSubjectStorage>());
}

TEST_F(SubjectControllerTest, getSubjects_Return2Subjects)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_EQ(2, controller->getSubjects().size());
	size_t index {0};
	for(const auto &subject : controller->getSubjects()) {
		if (index == 0) {
			ASSERT_EQ("Math", subject.getName());
			ASSERT_EQ(true, subject.getIsDefault());
		}
		else if (index == 1) {
			ASSERT_EQ("English", subject.getName());
			ASSERT_EQ(false, subject.getIsDefault());
		}
		index++;
	}
}

TEST_F(SubjectControllerTest, findSubject_WithIdZero_ReturnNullPtr) 
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_EQ(nullptr, controller->findSubject(0));
}

TEST_F(SubjectControllerTest, findSubject_WithIdOne_ReturnMath) 
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_NE(nullptr, controller->findSubject(1));
	ASSERT_EQ("Math", controller->findSubject(1)->getName());
	ASSERT_EQ(true, controller->findSubject(1)->getIsDefault());
}

TEST_F(SubjectControllerTest, findSubject_WithIdTwo_ReturnEnglish) 
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_NE(nullptr, controller->findSubject(2));
	ASSERT_EQ("English", controller->findSubject(2)->getName());
	ASSERT_EQ(false, controller->findSubject(2)->getIsDefault());
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("History"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Math"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("MATh"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Math"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Math   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Math   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd(""));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("History", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Math", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Math", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("MaTH", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("MAtH", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Math", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("   Math", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Math   ", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Math   ", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSubjects();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   ", 1));
}

TEST_F(SubjectControllerTest, insertSubject_WithSubjectThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertSubject(Subject { "Chicago" }));
}

TEST_F(SubjectControllerTest, insertSubject_WithSubjectThatWillFailed_ReturnFailed) 
{
	auto fakeSubjectStorage = dynamic_cast<FakeSubjectStorage*>(this->fakeStorage.get());
	fakeSubjectStorage->insertResult = false;
	fakeSubjectStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertSubject(Subject { "History" }));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(SubjectControllerTest, updateSubject_WithSubjectThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateSubject(Subject { "Math" }));
}

TEST_F(SubjectControllerTest, updateSubject_WithSubjectThatWillFailed_ReturnFailed) 
{
	auto fakeSubjectStorage = dynamic_cast<FakeSubjectStorage*>(this->fakeStorage.get());
	fakeSubjectStorage->updateResult = false;
	fakeSubjectStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateSubject(Subject { "History" }));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(SubjectControllerTest, deleteSubject_WithSubjectThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteSubject(1));
}

TEST_F(SubjectControllerTest, deleteSubject_WithSubjectThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeSubjectStorage = dynamic_cast<FakeSubjectStorage*>(this->fakeStorage.get());
	fakeSubjectStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteSubject(1));
	ASSERT_EQ("Unable to delete the subject because it is used by another item. (Probably a test)", controller->getLastError());
}

TEST_F(SubjectControllerTest, deleteSubject_WithSubjectThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeSubjectStorage = dynamic_cast<FakeSubjectStorage*>(this->fakeStorage.get());
	fakeSubjectStorage->deleteResult = QueryResult::ERROR;
	fakeSubjectStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteSubject(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}