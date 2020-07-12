#pragma once

#include "city.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class CityController
{
public:
    explicit CityController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<City>> managementItemStorage = nullptr);
    const std::list<City> &getCities() const;
    const City* findCity(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
						 size_t currentlyEditedCityId) const;
    const std::string &getLastError() const;
    void loadCities();
    bool insertCity(const City &city);
    bool updateCity(const City &city);
    bool deleteCity(size_t id);
private:
	std::list<City> cities;
    std::unique_ptr<IManagementItemStorage<City>> storage;
    std::string lastError;
};