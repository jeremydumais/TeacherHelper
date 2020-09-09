#include "fakeStudentStorage.h"
#include "studentSelectionController.h"
#include <gtest/gtest.h>

/*class FakeStudentController : public IStudentController
{
public:
    FakeStudentController()
		: students(std::list<Student> {
			Student(1, "Joe", "Blow"),
			Student(2, "Jane", "Doe", "A comment")
		}) {}
    std::list<Student> getAllItems() override { return students;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Student &student) override { return insertResult; }
    bool updateItem(const Student &student) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<Student> students;
};*/

TEST(StudentSelectionController_Constructor, ValidArguments_ReturnSuccess)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
}

TEST(StudentSelectionController_isStudentInFilter, EmptyFilter_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, OnlyPartFirstName_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("JO", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, FullFirstName_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("JOE", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, FullFirstNamePlusOneChar_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_FALSE(controller.isStudentInFilter("JOEL", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, OnlyPartLastName_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("BL", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, FullLastName_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("BLOW", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, FullLastNamePlusOneChar_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_FALSE(controller.isStudentInFilter("BLOWW", Student("Joe", "Blow")));
}

TEST(StudentSelectionController_isStudentInFilter, OnlyPartComments_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("TE", Student("Joe", "Blow", "Test")));
}

TEST(StudentSelectionController_isStudentInFilter, FullComments_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_TRUE(controller.isStudentInFilter("TEST", Student("Joe", "Blow", "Test")));
}

TEST(StudentSelectionController_isStudentInFilter, FullCommentsPlusOneChar_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_FALSE(controller.isStudentInFilter("TESTL", Student("Joe", "Blow", "Test")));
}

TEST(StudentSelectionController_isStudentInFilter, NonPresentChar_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_FALSE(controller.isStudentInFilter("Y", Student("Joe", "Blow", "Test")));
}

TEST(StudentSelectionController_isStudentInFilter, ContainsAll2FirstWordsButNotThird_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"));
    ASSERT_FALSE(controller.isStudentInFilter("JO BL AA", Student("Joe", "Blow", "Test")));
}