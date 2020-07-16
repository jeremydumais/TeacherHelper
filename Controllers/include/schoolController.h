#pragma once

#include "school.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class SchoolController
{
public:
    explicit SchoolController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<School>> managementItemStorage = nullptr);
    const std::list<School> &getSchools() const;
    const School* findSchool(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
						 size_t currentlyEditedCityId) const;
    const std::string &getLastError() const;
    void loadSchools();
    bool insertSchool(const School &school);
    bool updateSchool(const School &school);
    bool deleteSchool(size_t id);
private:
	std::list<School> schools;
    std::unique_ptr<IManagementItemStorage<School>> storage;
    std::string lastError;
};