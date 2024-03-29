#include "cityController.h"
#include "cityStorage.h"
#include <boost/algorithm/string.hpp>

using namespace std;

CityController::CityController(const IDatabaseController &databaseController,
                               unique_ptr<ManagementItemStorageBase<City>> managementItemStorage)
    : cities(list<City>()),
      storage { managementItemStorage ? 
                move(managementItemStorage) : 
                unique_ptr<ManagementItemStorageBase<City>>(make_unique<CityStorage>(databaseController.getDatabaseConnection()))},
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
    if (boost::trim_copy(name).empty()) {
        return false;
    }

    bool problemFound = false;
	for(const auto &city : cities) {
		if (boost::to_upper_copy(boost::trim_copy(city.getName())) == boost::to_upper_copy(boost::trim_copy(name))) {
			problemFound = true;
			break;
		}
	}
	return !problemFound;
}

bool CityController::isNewNameAvailableForUpdate(const string &name,
								 					 size_t currentlyEditedCityId) const
{
    if (boost::trim_copy(name).empty()) {
        return false;
    }

	bool problemFound = false;
	for(const auto &city : cities) {
		if (boost::to_upper_copy(boost::trim_copy(city.getName())) == boost::to_upper_copy(boost::trim_copy(name)) && city.getId() != currentlyEditedCityId) {
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
	cities = storage->getAllItems();
}

bool CityController::insertCity(const City &city)
{
    bool retVal = storage->insertItem(city);
    if (retVal) {
        loadCities();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}

bool CityController::updateCity(const City &city)
{
    bool retVal = storage->updateItem(city);
    if (retVal) {
        loadCities();
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal;
}


bool CityController::deleteCity(size_t id)
{
    QueryResult retVal = storage->deleteItem(id);
    if (retVal == QueryResult::OK) {
        loadCities();
    }
    else if (retVal == QueryResult::CONSTRAINTERROR) {
        lastError = "Unable to delete the city because it is used by another item. (Probably a school)";
    }
    else {
        lastError = storage->getLastError();
    }
    return retVal == QueryResult::OK;
}
