#pragma once

#include "fakeDatabaseConnection.h"
#include "ManagementItemStorageBase.h"
#include "student.h"
#include <list>

class FakeStudentStorage : public ManagementItemStorageBase<Student>
{
public:
    FakeStudentStorage()
		: students(std::list<Student> {
			Student(1, "Joe", "Blow"),
			Student(2, "Jane", "Doe", "A comment")
		}),
		ManagementItemStorageBase<Student>(FakeDatabaseConnection()) {}
    std::list<Student> getAllItems() override { return students;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const Student &student) override { return insertResult; }
    bool updateItem(const Student &student) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool isReferentialIntegrityConstraint(size_t id) override { return true; };
	std::string getSelectCommand() const override { return ""; };
    Student getItemFromRecord(const IStorageSelectOperation &record) const override { return { 1, "FakeFirstName", "FakeLastName" }; };
    std::string getInsertCommand() const override { return ""; };
    std::vector<std::string> getInsertValues(const Student &item) const override { return {"", ""}; };
    std::string getUpdateCommand() const override {return ""; };
    std::vector<std::string> getUpdateValues(const Student &item) const override { return {"", ""}; };
    std::string getDeleteCommand() const override {return ""; };
    std::vector<std::string> getDeleteValues(size_t id) const override { return {""}; };
    std::string getReferentialIntegrityConstraintsCommand() const override { return ""; };
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override { return {""}; };
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<Student> students;
};