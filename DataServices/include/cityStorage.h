#pragma once

#include "city.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include <list>

class CityStorage : public IManagementItemStorage<City>
{
public:
    explicit CityStorage(const DatabaseConnection &connection);
    std::list<City> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const City &city) override;
    bool updateItem(const City &city) override;
    bool deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};