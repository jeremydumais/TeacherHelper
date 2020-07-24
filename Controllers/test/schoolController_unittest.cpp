#include "schoolController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeSchoolStorage : public IManagementItemStorage<School>
{
public:
    FakeSchoolStorage()
		: schools(std::list<School> {
			School(1, "Thomas Jefferson High School", City("Alexandria")),
			School(2, "Carnegie Vanguard High School", City("Houston"))
		}) {}
    std::list<School> getAllItems() override { return schools;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const School &school) override { return true; }
    bool updateItem(const School &school) override { return true; }
    QueryResult deleteItem(size_t id) override { return QueryResult::OK; }
private:
	std::list<School> schools;
    std::string lastError;
};

class SchoolControllerTest : public ::testing::Test
{
public:
	SchoolControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<School>>(make_unique<FakeSchoolStorage>()))
	{}
	SchoolController controller;
};

TEST(SchoolController_Constructor, ValidArguments_ReturnSuccess)
{
	SchoolController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<School>>(make_unique<FakeSchoolStorage>()));
}

TEST_F(SchoolControllerTest, getSchools_Return2Schools)
{
	controller.loadSchools();
	ASSERT_EQ(2, controller.getSchools().size());
	size_t index {0};
	for(const auto &school : controller.getSchools()) {
		if (index == 0) {
			ASSERT_EQ("Thomas Jefferson High School", school.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Carnegie Vanguard High School", school.getName());
		}
		index++;
	}
}

TEST_F(SchoolControllerTest, findSchool_WithIdZero_ReturnNullPtr) 
{
	controller.loadSchools();
	ASSERT_EQ(nullptr, controller.findSchool(0));
}

TEST_F(SchoolControllerTest, findSchool_WithIdOne_ReturnNewYork) 
{
	controller.loadSchools();
	ASSERT_NE(nullptr, controller.findSchool(1));
	ASSERT_EQ("Thomas Jefferson High School", controller.findSchool(1)->getName());
}

TEST_F(SchoolControllerTest, findSchool_WithIdTwo_ReturnNewYork) 
{
	controller.loadSchools();
	ASSERT_NE(nullptr, controller.findSchool(2));
	ASSERT_EQ("Carnegie Vanguard High School", controller.findSchool(2)->getName());
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	controller.loadSchools();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("Another High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Thomas Jefferson High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Thomas JeFFerson HIGH School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Thomas Jefferson High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("Thomas Jefferson High School   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   Thomas Jefferson High School   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd(""));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	controller.loadSchools();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Another High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	controller.loadSchools();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Thomas Jefferson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Thomas Jefferson High School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Thomas JeFFerson HIGH School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	controller.loadSchools();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("ThOMAS JeFFerson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Thomas Jefferson High School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	controller.loadSchools();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("   Thomas Jefferson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("Thomas Jefferson High School   ", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   Thomas Jefferson High School   ", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	controller.loadSchools();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   ", 1));
}