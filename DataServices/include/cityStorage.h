#pragma once

#include "city.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "sqliteDeleteOperation.h"
#include <list>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define CITYSTORAGE_API __declspec(dllexport)   
    #else  
        #define CITYSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define CITYSTORAGE_API
#endif

class CITYSTORAGE_API CityStorage : public IManagementItemStorage<City>
{
public:
    explicit CityStorage(const DatabaseConnection &connection);
    std::list<City> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const City &city) override;
    bool updateItem(const City &city) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};