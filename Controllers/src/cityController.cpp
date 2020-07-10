#include "cityController.h"
#include <boost/algorithm/string.hpp>

using namespace std;

CityController::CityController(const DatabaseConnection &dbConnection)
    : dbConnection(&dbConnection),
      cities(list<City>()),
      storage(dbConnection),
      lastError("")
{
}

const std::list<City> &CityController::getCities() const
{
    return cities;
}

const City *CityController::findCity(size_t id) const
{
	const City *retVal = nullptr;
	for(const auto &city : cities) {
		if (city.getId() == id) {
			retVal = &city; 
			break;
		}
	}
	return retVal;
}

bool CityController::isNewNameAvailableForAdd(const string &name) const
{
	return _isNewNameAvailable(name, -1, false);
}

bool CityController::isNewNameAvailableForUpdate(const string &name,
								 					 size_t currentlyEditedCityId) const
{
	return _isNewNameAvailable(name, currentlyEditedCityId, true);
}

bool CityController::_isNewNameAvailable(const string &name, 
										 	 size_t currentlyEditedCityId, 
										 	 bool isUpdateMode) const
{
	bool problemFound = false;
	for(const auto &city : cities) {
		if ((boost::to_upper_copy(city.getName()) == boost::to_upper_copy(name) && !isUpdateMode) || 
			(boost::to_upper_copy(city.getName()) == boost::to_upper_copy(name) && isUpdateMode && city.getId() != currentlyEditedCityId)) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

const std::string &CityController::getLastError() const
{
    return lastError;
}


void CityController::loadCities()
{
	cities = storage.getAllCities();
}

bool CityController::insertCity(const City &city)
{
    bool retVal = storage.insertCity(city);
    if (retVal) {
        loadCities();
    }
    else {
        lastError = storage.getLastError();
    }
    return retVal;
}

bool CityController::updateCity(const City &city)
{
    bool retVal = storage.updateCity(city);
    if (retVal) {
        loadCities();
    }
    else {
        lastError = storage.getLastError();
    }
    return retVal;
}


bool CityController::deleteCity(size_t id)
{
    bool retVal = storage.deleteCity(id);
    if (retVal) {
        loadCities();
    }
    else {
        lastError = storage.getLastError();
    }
    return retVal;
}
