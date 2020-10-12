#include "fakeStudentStorage.h"
#include "studentController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class StudentControllerTest : public ::testing::Test
{
public:
	StudentControllerTest() 
		: fakeStorage { make_unique<FakeStudentStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<StudentController>(DatabaseConnection("nulldb"), 
												 std::move(fakeStorage));
	}

	unique_ptr<ManagementItemStorageBase<Student>> fakeStorage;								 
	unique_ptr<StudentController> controller;
};

TEST(StudentController_Constructor, ValidArguments_ReturnSuccess)
{
	StudentController controller(DatabaseConnection("nulldb"), unique_ptr<ManagementItemStorageBase<Student>>(make_unique<FakeStudentStorage>()));
}

TEST_F(StudentControllerTest, getStudents_Return2Students)
{
	this->prepareController();
	controller->loadStudents();
	ASSERT_EQ(2, controller->getStudents().size());
	size_t index {0};
	for(const auto &student : controller->getStudents()) {
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
	this->prepareController();
	controller->loadStudents();
	ASSERT_EQ(nullptr, controller->findStudent(0));
}

TEST_F(StudentControllerTest, findStudent_WithIdOne_ReturnJoeBlow) 
{
	this->prepareController();
	controller->loadStudents();
	const Student *const actual = controller->findStudent(1);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Joe", actual->getFirstName());
	ASSERT_EQ("Blow", actual->getLastName());
}

TEST_F(StudentControllerTest, findStudent_WithIdTwo_ReturnJaneDoe) 
{
	this->prepareController();
	controller->loadStudents();
	const Student *const actual = controller->findStudent(2);
	ASSERT_NE(nullptr, actual);
	ASSERT_EQ("Jane", actual->getFirstName());
	ASSERT_EQ("Doe", actual->getLastName());
	ASSERT_EQ("A comment", actual->getComments());
}







TEST_F(StudentControllerTest, insertStudent_WithStudentThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertStudent(Student("Santa", "Claus")));
}

TEST_F(StudentControllerTest, insertStudent_WithStudentThatWillFailed_ReturnFailed) 
{
	auto fakeStudentStorage = dynamic_cast<FakeStudentStorage*>(this->fakeStorage.get());
	fakeStudentStorage->insertResult = false;
	fakeStudentStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertStudent(Student("Santa", "Claus")));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(StudentControllerTest, updateStudent_WithStudentThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateStudent(Student(1, "Joe", "Blow")));
}

TEST_F(StudentControllerTest, updateStudent_WithStudentThatWillFailed_ReturnFailed) 
{
	auto fakeStudentStorage = dynamic_cast<FakeStudentStorage*>(this->fakeStorage.get());
	fakeStudentStorage->updateResult = false;
	fakeStudentStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateStudent(Student("Santa", "Claus")));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(StudentControllerTest, deleteStudent_WithStudentThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteStudent(1));
}

TEST_F(StudentControllerTest, deleteStudent_WithStudentThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeStudentStorage = dynamic_cast<FakeStudentStorage*>(this->fakeStorage.get());
	fakeStudentStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteStudent(1));
	ASSERT_EQ("Unable to delete the student because it is used by another item. (Probably a class or an assessment result)", controller->getLastError());
}

TEST_F(StudentControllerTest, deleteStudent_WithStudentThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeStudentStorage = dynamic_cast<FakeStudentStorage*>(this->fakeStorage.get());
	fakeStudentStorage->deleteResult = QueryResult::ERROR;
	fakeStudentStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteStudent(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}