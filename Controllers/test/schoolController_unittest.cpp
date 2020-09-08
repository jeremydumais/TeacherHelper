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
    bool insertItem(const School &school) override { return insertResult; }
    bool updateItem(const School &school) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<School> schools;
};

class SchoolControllerTest : public ::testing::Test
{
public:
	SchoolControllerTest() 
		: fakeStorage { make_unique<FakeSchoolStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<SchoolController>(DatabaseConnection("nulldb"), 
												 std::move(fakeStorage));
	}

	unique_ptr<IManagementItemStorage<School>> fakeStorage;								 
	unique_ptr<SchoolController> controller;
};

TEST(SchoolController_Constructor, ValidArguments_ReturnSuccess)
{
	SchoolController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<School>>(make_unique<FakeSchoolStorage>()));
}

TEST_F(SchoolControllerTest, getSchools_Return2Schools)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_EQ(2, controller->getSchools().size());
	size_t index {0};
	for(const auto &school : controller->getSchools()) {
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
	this->prepareController();
	controller->loadSchools();
	ASSERT_EQ(nullptr, controller->findSchool(0));
}

TEST_F(SchoolControllerTest, findSchool_WithIdOne_ReturnNewYork) 
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_NE(nullptr, controller->findSchool(1));
	ASSERT_EQ("Thomas Jefferson High School", controller->findSchool(1)->getName());
}

TEST_F(SchoolControllerTest, findSchool_WithIdTwo_ReturnNewYork) 
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_NE(nullptr, controller->findSchool(2));
	ASSERT_EQ("Carnegie Vanguard High School", controller->findSchool(2)->getName());
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("Another High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Thomas Jefferson High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Thomas JeFFerson HIGH School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Thomas Jefferson High School"));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("Thomas Jefferson High School   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   Thomas Jefferson High School   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd(""));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   "));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Another High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Thomas Jefferson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Thomas Jefferson High School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Thomas JeFFerson HIGH School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("ThOMAS JeFFerson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Thomas Jefferson High School", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("   Thomas Jefferson High School", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("Thomas Jefferson High School   ", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   Thomas Jefferson High School   ", 2));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("", 1));
}

TEST_F(SchoolControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadSchools();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   ", 1));
}


TEST_F(SchoolControllerTest, insertSchool_WithSchoolThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertSchool(School("A New School", City("Chicago"))));
}

TEST_F(SchoolControllerTest, insertSchool_WithSchoolThatWillFailed_ReturnFailed) 
{
	auto fakeSchoolStorage = dynamic_cast<FakeSchoolStorage*>(this->fakeStorage.get());
	fakeSchoolStorage->insertResult = false;
	fakeSchoolStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertSchool(School("A New School", City("Chicago"))));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(SchoolControllerTest, updateSchool_WithSchoolThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateSchool(School(1, "Thomas Jefferson High School", City("Alexandria"))));
}

TEST_F(SchoolControllerTest, updateSchool_WithSchoolThatWillFailed_ReturnFailed) 
{
	auto fakeSchoolStorage = dynamic_cast<FakeSchoolStorage*>(this->fakeStorage.get());
	fakeSchoolStorage->updateResult = false;
	fakeSchoolStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateSchool(School("A New School", City("Chicago"))));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(SchoolControllerTest, deleteSchool_WithSchoolThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteSchool(1));
}

TEST_F(SchoolControllerTest, deleteSchool_WithSchoolThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeSchoolStorage = dynamic_cast<FakeSchoolStorage*>(this->fakeStorage.get());
	fakeSchoolStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteSchool(1));
	ASSERT_EQ("Unable to delete the school because it is used by another item. (Probably a class)", controller->getLastError());
}

TEST_F(SchoolControllerTest, deleteSchool_WithSchoolThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeSchoolStorage = dynamic_cast<FakeSchoolStorage*>(this->fakeStorage.get());
	fakeSchoolStorage->deleteResult = QueryResult::ERROR;
	fakeSchoolStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteSchool(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}
