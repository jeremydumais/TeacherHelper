#pragma once

#include "city.h"
#include "cityStorage.h"
#include "databaseConnection.h"
#include <list>

class CityController
{
public:
    explicit CityController(const DatabaseConnection &dbConnection);
    const std::list<City> &getCities() const;
    const City* findCity(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
						 size_t currentlyEditedCityId) const;
	bool _isNewNameAvailable(const std::string &name, 
						 size_t currentlyEditedCityId, 
						 bool isUpdateMode) const;
    const std::string &getLastError() const;
    void loadCities();
    bool insertCity(const City &city);
    bool updateCity(const City &city);
    bool deleteCity(size_t id);
private:
    const DatabaseConnection * const dbConnection;
	std::list<City> cities;
    CityStorage storage;
    std::string lastError;
};