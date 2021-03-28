#pragma once

#include "city.h"
#include "ManagementItemStorageBase.h"
#include "IDatabaseController.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define CITYCONTROLLER_API __declspec(dllexport)   
    #else  
        #define CITYCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define CITYCONTROLLER_API
#endif

class CITYCONTROLLER_API CityController
{
public:
    explicit CityController(const IDatabaseController &databaseController,
                            std::unique_ptr<ManagementItemStorageBase<City>> managementItemStorage = nullptr);
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
    std::unique_ptr<ManagementItemStorageBase<City>> storage;
    std::string lastError;
};