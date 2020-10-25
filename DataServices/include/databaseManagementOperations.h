#pragma once

#include "IDatabaseManagementOperations.h"
#include <string>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define DATABASEMANAGEMENTOPERATIONS_API __declspec(dllexport)   
    #else  
        #define DATABASEMANAGEMENTOPERATIONS_API __declspec(dllimport)   
    #endif
#else
    #define DATABASEMANAGEMENTOPERATIONS_API
#endif

class DATABASEMANAGEMENTOPERATIONS_API DatabaseManagementOperations : public IDatabaseManagementOperations
{
public:
    bool backup(const std::string &databaseName, const std::string &backupDatabaseName) override; 
private:
    std::string lastError;
};