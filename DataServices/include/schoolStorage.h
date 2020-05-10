#pragma once

#include "databaseConnection.h"
#include "school.h"
#include <list>

class SchoolStorage
{
public:
    SchoolStorage(const DatabaseConnection &connection);
    std::list<School> getAllSchools();
    const std::string &getLastError() const;
    bool insertSchool(const School &school);
    bool updateSchool(const School &school);
    bool deleteSchool(size_t id);
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};