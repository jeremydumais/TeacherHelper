#include "classController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeClassStorage : public ManagementItemStorageBase<Class>
{
public:
    FakeClassStorage()
		: classes(std::list<Class> {
			Class(1, "First Grade", School(1, "Thomas Jefferson High School", City("Alexandria"))),
			Class(2, "Second Grade", School(2, "Carnegie Vanguard High School", City("Houston")))
		}),
		ManagementItemStorageBase<Class>(DatabaseConnection("nulldb")) {}
    std::list<Class> getAllItems() override { return classes;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Class &p_class) override { return insertResult; }
    bool updateItem(const Class &p_class) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool isReferentialIntegrityConstraint(size_t id) override { return true; };
	std::string getSelectCommand() const override { return ""; };
    Class getItemFromRecord(const IStorageSelectOperation &record) const override { return { 1, "FakeClass", School(1, "FakeSchool", City(1, "FakeCity")) }; };
    std::string getInsertCommand() const override { return ""; };
    std::vector<std::string> getInsertValues(const Class &item) const override { return {"", ""}; };
    std::string getUpdateCommand() const override {return ""; };
    std::vector<std::string> getUpdateValues(const Class &item) const override { return {"", ""}; };
    std::string getDeleteCommand() const override {return ""; };
    std::vector<std::string> getDeleteValues(size_t id) const override { return {""}; };
    std::string getReferentialIntegrityConstraintsCommand() const override { return ""; };
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override { return {""}; };
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<Class> classes;
};

class ClassControllerTest : public ::testing::Test
{
public:
	ClassControllerTest() 
		: fakeStorage { make_unique<FakeClassStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<ClassController>(DatabaseConnection("nulldb"), 
												 std::move(fakeStorage));
	}

	unique_ptr<ManagementItemStorageBase<Class>> fakeStorage;								 
	unique_ptr<ClassController> controller;
};

TEST(ClassController_Constructor, ValidArguments_ReturnSuccess)
{
	ClassController controller(DatabaseConnection("nulldb"), unique_ptr<ManagementItemStorageBase<Class>>(make_unique<FakeClassStorage>()));
}

TEST_F(ClassControllerTest, getClasses_Return2Classes)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_EQ(2, controller->getClasses().size());
	size_t index {0};
	for(const auto &aClass : controller->getClasses()) {
		if (index == 0) {
			ASSERT_EQ("First Grade", aClass.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Second Grade", aClass.getName());
		}
		index++;
	}
}

TEST_F(ClassControllerTest, getClassesBySchoolWithSchool1_Return1Class)
{
	this->prepareController();
	controller->loadClasses();
	auto classes = controller->getClassesBySchool(School(1, "Thomas Jefferson High School", City("Alexandria")));
	ASSERT_EQ(1, classes.size());
	ASSERT_EQ("First Grade", classes.begin()->getName());
}

TEST_F(ClassControllerTest, getClassesBySchoolWithSchool2_Return1Class)
{
	this->prepareController();
	controller->loadClasses();
	auto classes = controller->getClassesBySchool(School(2, "Carnegie Vanguard High School", City("Houston")));
	ASSERT_EQ(1, classes.size());
	ASSERT_EQ("Second Grade", classes.begin()->getName());
}

TEST_F(ClassControllerTest, getClassesBySchoolWithNotExistantSchool_Return1Class)
{
	this->prepareController();
	controller->loadClasses();
	auto classes = controller->getClassesBySchool(School(3, "Not existing school", City("Houston")));
	ASSERT_EQ(0, classes.size());
}

TEST_F(ClassControllerTest, findClass_WithIdZero_ReturnNullPtr) 
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_EQ(nullptr, controller->findClass(0));
}

TEST_F(ClassControllerTest, findClass_WithIdOne_ReturnFirstGrade) 
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_NE(nullptr, controller->findClass(1));
	ASSERT_EQ("First Grade", controller->findClass(1)->getName());
}

TEST_F(ClassControllerTest, findClass_WithIdTwo_ReturnSecondGrade) 
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_NE(nullptr, controller->findClass(2));
	ASSERT_EQ("Second Grade", controller->findClass(2)->getName());
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("Another Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("First Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCaseSameSchool_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("FiRST Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameButDifferentSchool_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("First Grade", 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   First Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("First Grade   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   First Grade   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Another Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("First Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("First Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("FIRST Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("FIRST Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   FIRST Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("   First Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("First Grade   ", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   First Grade   ", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadClasses();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   ", 1, 1));
}

TEST_F(ClassControllerTest, insertClass_WithClassThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertClass(Class(1, "Third Grade", School(1, "Thomas Jefferson High School", City("Alexandria")))));
}

TEST_F(ClassControllerTest, insertClass_WithClassThatWillFailed_ReturnFailed) 
{
	auto fakeClassStorage = dynamic_cast<FakeClassStorage*>(this->fakeStorage.get());
	fakeClassStorage->insertResult = false;
	fakeClassStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertClass(Class(1, "Third Grade", School(1, "Thomas Jefferson High School", City("Alexandria")))));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(ClassControllerTest, updateClass_WithClassThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateClass(Class(1, "First Grade", School(1, "Thomas Jefferson High School", City("Alexandria")))));
}

TEST_F(ClassControllerTest, updateClass_WithClassThatWillFailed_ReturnFailed) 
{
	auto fakeClassStorage = dynamic_cast<FakeClassStorage*>(this->fakeStorage.get());
	fakeClassStorage->updateResult = false;
	fakeClassStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateClass(Class(1, "Third Grade", School(1, "Thomas Jefferson High School", City("Alexandria")))));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(ClassControllerTest, deleteClass_WithClassThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteClass(1));
}

TEST_F(ClassControllerTest, deleteClass_WithClassThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeClassStorage = dynamic_cast<FakeClassStorage*>(this->fakeStorage.get());
	fakeClassStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteClass(1));
	ASSERT_EQ("Unable to delete the class because it is used by another item.", controller->getLastError());
}

TEST_F(ClassControllerTest, deleteClass_WithClassThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeClassStorage = dynamic_cast<FakeClassStorage*>(this->fakeStorage.get());
	fakeClassStorage->deleteResult = QueryResult::ERROR;
	fakeClassStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteClass(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}