#include "subjectController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeSubjectStorage : public IManagementItemStorage<Subject>
{
public:
    FakeSubjectStorage()
		: subjects(std::list<Subject> {
			Subject(1, "Math", true),
			Subject(2, "English")
		}) {}
    std::list<Subject> getAllItems() override { return subjects;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Subject &subject) override { return true; }
    bool updateItem(const Subject &subject) override { return true; }
    QueryResult deleteItem(size_t id) override { return QueryResult::OK; }
private:
	std::list<Subject> subjects;
    std::string lastError;
};

class SubjectControllerTest : public ::testing::Test
{
public:
	SubjectControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<Subject>>(make_unique<FakeSubjectStorage>()))
	{}
	SubjectController controller;
};

TEST(SubjectController_Constructor, ValidArguments_ReturnSuccess)
{
	SubjectController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<Subject>>(make_unique<FakeSubjectStorage>()));
}

TEST_F(SubjectControllerTest, getSubjects_Return2Subjects)
{
	controller.loadSubjects();
	ASSERT_EQ(2, controller.getSubjects().size());
	size_t index {0};
	for(const auto &subject : controller.getSubjects()) {
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
	controller.loadSubjects();
	ASSERT_EQ(nullptr, controller.findSubject(0));
}

TEST_F(SubjectControllerTest, findSubject_WithIdOne_ReturnMath) 
{
	controller.loadSubjects();
	ASSERT_NE(nullptr, controller.findSubject(1));
	ASSERT_EQ("Math", controller.findSubject(1)->getName());
	ASSERT_EQ(true, controller.findSubject(1)->getIsDefault());
}

TEST_F(SubjectControllerTest, findSubject_WithIdTwo_ReturnEnglish) 
{
	controller.loadSubjects();
	ASSERT_NE(nullptr, controller.findSubject(2));
	ASSERT_EQ("English", controller.findSubject(2)->getName());
	ASSERT_EQ(false, controller.findSubject(2)->getIsDefault());
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	controller.loadSubjects();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("History"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Math"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("MATh"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Math"));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Math   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Math   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd(""));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   "));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	controller.loadSubjects();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("History", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	controller.loadSubjects();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Math", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Math", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("MaTH", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	controller.loadSubjects();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("MAtH", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Math", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	controller.loadSubjects();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("   Math", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Math   ", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Math   ", 2));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("", 1));
}

TEST_F(SubjectControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	controller.loadSubjects();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   ", 1));
}

