#pragma once

#include "class.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class ClassController
{
public:
    explicit ClassController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<Class>> managementItemStorage = nullptr);
    const std::list<Class> &getClasses() const;
    const Class* findClass(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name, const size_t school_id) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
                         const size_t school_id,
						 size_t currentlyEditedClassId) const;
    const std::string &getLastError() const;
    void loadClasses();
    bool insertClass(const Class &p_class);
    bool updateClass(const Class &p_class);
    bool deleteClass(size_t id);
private:
	std::list<Class> classes;
    std::unique_ptr<IManagementItemStorage<Class>> storage;
    std::string lastError;
};