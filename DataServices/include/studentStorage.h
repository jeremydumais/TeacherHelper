#pragma once

#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "student.h"
#include <list>

class StudentStorage : public IManagementItemStorage<Student>
{
public:
    explicit StudentStorage(const DatabaseConnection &connection);
    std::list<Student> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Student &student) override;
    bool updateItem(const Student &student) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};