#pragma once

#include "databaseConnection.h"
#include "city.h"
#include <list>

class CityStorage
{
public:
    explicit CityStorage(const DatabaseConnection &connection);
    std::list<City> getAllCities();
    const std::string &getLastError() const;
    bool insertCity(const City &city);
    bool updateCity(const City &city);
    bool deleteCity(size_t id);
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};