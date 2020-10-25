#pragma once

#include "version.h"
#include "IDatabaseConnection.h"
#include "IDatabaseManagementOperations.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define DATABASECONTROLLER_API __declspec(dllexport)   
    #else  
        #define DATABASECONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define DATABASECONTROLLER_API
#endif

class DATABASECONTROLLER_API DatabaseController
{
public:
    explicit DatabaseController(std::unique_ptr<IDatabaseConnection> databaseConnection = nullptr,
                                std::unique_ptr<IDatabaseManagementOperations> databaseManagementOperations = nullptr);
    bool isDatabaseOpened() const;
    std::string getOpenedDatabaseName() const;
    void openDatabase(const std::string &databaseName);
    void closeDatabase();
        
private:
    std::unique_ptr<IDatabaseConnection> databaseConnection;
    std::unique_ptr<IDatabaseManagementOperations> databaseManagementOperations;
};