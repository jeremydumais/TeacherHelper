#include "studentController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeStudentStorage : public IManagementItemStorage<Student>
{
public:
    FakeStudentStorage()
		: students(std::list<Student> {
			Student(1, "Joe", "Blow"),
			Student(2, "Jane", "Doe", "A comment")
		}) {}
    std::list<Student> getAllItems() override { return students;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Student &student) override { return true; }
    bool updateItem(const Student &student) override { return true; }
    bool deleteItem(size_t id) override { return true; }
private:
	std::list<Student> students;
    std::string lastError;
};

class StudentControllerTest : public ::testing::Test
{
public:
	StudentControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<Student>>(make_unique<FakeStudentStorage>()))
	{}
	StudentController controller;
};

TEST(StudentController_Constructor, ValidArguments_ReturnSuccess)
{
	StudentController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<Student>>(make_unique<FakeStudentStorage>()));
}

TEST_F(StudentControllerTest, getStudents_Return2Students)
{
	controller.loadStudents();
	ASSERT_EQ(2, controller.getStudents().size());
	size_t index {0};
	for(const auto &student : controller.getStudents()) {
		if (index == 0) {
			ASSERT_EQ("Joe", student.getFirstName());
			ASSERT_EQ("Blow", student.getLastName());
		}
		else if (index == 1) {
			ASSERT_EQ("Jane", student.getFirstName());
			ASSERT_EQ("Doe", student.getLastName());
			ASSERT_EQ("A comment", student.getComments());
		}
		index++;
	}
}

TEST_F(StudentControllerTest, findStudent_WithIdZero_ReturnNullPtr) 
{
	controller.loadStudents();
	ASSERT_EQ(nullptr, controller.findStudent(0));
}

TEST_F(StudentControllerTest, findStudent_WithIdOne_ReturnJoeBlow) 
{
	controller.loadStudents();
	const Student *const actual = controller.findStudent(1);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Joe", actual->getFirstName());
	ASSERT_EQ("Blow", actual->getLastName());
}

TEST_F(StudentControllerTest, findStudent_WithIdTwo_ReturnJaneDoe) 
{
	controller.loadStudents();
	const Student *const actual = controller.findStudent(2);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Jane", actual->getFirstName());
	ASSERT_EQ("Doe", actual->getLastName());
	ASSERT_EQ("A comment", actual->getComments());
}
