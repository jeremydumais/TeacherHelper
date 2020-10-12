#include "fakeStudentStorage.h"
#include "studentSelectionController.h"
#include <gtest/gtest.h>

using namespace std;

class FakeStudentController : public IStudentController
{
public:
    FakeStudentController()
		: students { std::list<Student>() },
		  storage { make_unique<FakeStudentStorage>()} {}
    const std::list<Student> &getStudents() const override { return students; }
    const Student* findStudent(size_t id) const override { return findStudentResult.get(); }
    const std::string &getLastError() const override { return lastError; }
    void loadStudents() override { 
		students.emplace_back(1, "Joe", "Blow");
		students.emplace_back(2, "Jane", "Doe", "A comment");
		}
    bool insertStudent(const Student &student) override { return insertResult; }
    bool updateStudent(const Student &student) override { return updateResult; }
    bool deleteStudent(size_t id) override { return deleteResult; }
	
	std::list<Student> students;
    std::unique_ptr<ManagementItemStorageBase<Student>> storage;
    std::string lastError;
	std::unique_ptr<Student> findStudentResult;
	bool insertResult = true;
	bool updateResult = true;
	bool deleteResult = true;
};

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

TEST(StudentSelectionController_isStudentInFilter, ContainsAll2FirstWordsButNotThirdWithFakeStorage_ReturnTrue)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"), make_unique<FakeStudentController>());
    ASSERT_TRUE(controller.isStudentInFilter("TEST", Student("Joe", "Blow", "Test")));
}

TEST(StudentSelectionController_loadStudents, StandardCall_ReturnSuccess)
{
	StudentSelectionController controller(DatabaseConnection("nulldb"), make_unique<FakeStudentController>());
	ASSERT_EQ(0, controller.getStudents().size());
	controller.loadStudents();
	ASSERT_EQ(2, controller.getStudents().size());
}

TEST(StudentSelectionController_findStudent, StandardCall_ReturnSuccess)
{
	auto studentController { make_unique<FakeStudentController>() };
	studentController->findStudentResult = make_unique<Student>(3, "Santa", "Claus");
	StudentSelectionController controller(DatabaseConnection("nulldb"), move(studentController));
	auto expected = controller.findStudent(3);
	ASSERT_EQ("Santa", expected->getFirstName());
	ASSERT_EQ("Claus", expected->getLastName());
}