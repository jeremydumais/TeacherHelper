#pragma once

#include "IManagementItemStorage.h"
#include "student.h"
#include <list>

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
    bool insertItem(const Student &student) override { return insertResult; }
    bool updateItem(const Student &student) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<Student> students;
};