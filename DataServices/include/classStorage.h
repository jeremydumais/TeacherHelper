#pragma once

#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "class.h"
#include <list>

class ClassStorage : public IManagementItemStorage<Class>
{
public:
    explicit ClassStorage(const DatabaseConnection &connection);
    std::list<Class> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Class &p_class) override;
    bool updateItem(const Class &p_class) override;
    bool deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};