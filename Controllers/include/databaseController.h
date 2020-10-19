#pragma once

#include "version.h"
#include "databaseConnection.h"
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
    explicit DatabaseController(std::string dbName,
                                std::unique_ptr<IDatabaseConnection> databaseConnection = nullptr);
    const std::string &getDatabaseName() const;
    void openDatabase();
    void closeDatabase();
        
private:
    std::unique_ptr<IDatabaseConnection> databaseConnection;
};