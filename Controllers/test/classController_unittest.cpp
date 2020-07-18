#include "classController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeClassStorage : public IManagementItemStorage<Class>
{
public:
    FakeClassStorage()
		: classes(std::list<Class> {
			Class(1, "First Grade", School(1, "Thomas Jefferson High School", City("Alexandria"))),
			Class(2, "Second Grade", School(2, "Carnegie Vanguard High School", City("Houston")))
		}) {}
    std::list<Class> getAllItems() override { return classes;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Class &p_class) override { return true; }
    bool updateItem(const Class &p_class) override { return true; }
    bool deleteItem(size_t id) override { return true; }
private:
	std::list<Class> classes;
    std::string lastError;
};

class ClassControllerTest : public ::testing::Test
{
public:
	ClassControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<Class>>(make_unique<FakeClassStorage>()))
	{}
	ClassController controller;
};

TEST(ClassController_Constructor, ValidArguments_ReturnSuccess)
{
	ClassController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<Class>>(make_unique<FakeClassStorage>()));
}

TEST_F(ClassControllerTest, getClasses_Return2Classes)
{
	controller.loadClasses();
	ASSERT_EQ(2, controller.getClasses().size());
	size_t index {0};
	for(const auto &aClass : controller.getClasses()) {
		if (index == 0) {
			ASSERT_EQ("First Grade", aClass.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Second Grade", aClass.getName());
		}
		index++;
	}
}

TEST_F(ClassControllerTest, findClass_WithIdZero_ReturnNullPtr) 
{
	controller.loadClasses();
	ASSERT_EQ(nullptr, controller.findClass(0));
}

TEST_F(ClassControllerTest, findClass_WithIdOne_ReturnFirstGrade) 
{
	controller.loadClasses();
	ASSERT_NE(nullptr, controller.findClass(1));
	ASSERT_EQ("First Grade", controller.findClass(1)->getName());
}

TEST_F(ClassControllerTest, findClass_WithIdTwo_ReturnSecondGrade) 
{
	controller.loadClasses();
	ASSERT_NE(nullptr, controller.findClass(2));
	ASSERT_EQ("Second Grade", controller.findClass(2)->getName());
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("Another Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("First Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCaseSameSchool_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("FiRST Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameButDifferentSchool_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("First Grade", 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   First Grade", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("First Grade   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   First Grade   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   ", 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Another Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("First Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("First Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("FIRST Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("FIRST Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   FIRST Grade", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	controller.loadClasses();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("   First Grade", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("First Grade   ", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   First Grade   ", 1, 2));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("", 1, 1));
}

TEST_F(ClassControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	controller.loadClasses();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   ", 1, 1));
}