#pragma once

#include "databaseConnection.h"
#include "class.h"
#include <list>

class ClassStorage
{
public:
    explicit ClassStorage(const DatabaseConnection &connection);
    std::list<Class> getAllClasses();
    const std::string &getLastError() const;
    bool insertClass(const Class &p_class);
    bool updateClass(const Class &p_class);
    bool deleteClass(size_t id);
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};