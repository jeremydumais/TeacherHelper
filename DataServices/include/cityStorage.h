#pragma once

#include "city.h"
#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define CITYSTORAGE_API __declspec(dllexport)   
    #else  
        #define CITYSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define CITYSTORAGE_API
#endif

class CITYSTORAGE_API CityStorage : public ManagementItemStorageBase<City>
{
public:
    explicit CityStorage(const IDatabaseConnection &connection, 
                         const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);

    std::string getSelectCommand() const override;
    City getItemFromRecord(const IStorageSelectOperation &record) const override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const City &item) const override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const City &item) const override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
};