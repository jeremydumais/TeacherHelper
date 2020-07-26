#pragma once

#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "school.h"
#include <list>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SCHOOLSTORAGE_API __declspec(dllexport)   
    #else  
        #define SCHOOLSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define SCHOOLSTORAGE_API
#endif

class SCHOOLSTORAGE_API SchoolStorage : public IManagementItemStorage<School>
{
public:
    explicit SchoolStorage(const DatabaseConnection &connection);
    std::list<School> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const School &school) override;
    bool updateItem(const School &school) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
};