#pragma once

#include "IDatabaseOperations.h"
#include <string>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define SQLITEDATABASEOPERATIONS_API __declspec(dllexport)   
    #else  
        #define SQLITEDATABASEOPERATIONS_API __declspec(dllimport)   
    #endif
#else
    #define SQLITEDATABASEOPERATIONS_API
#endif

class SQLITEDATABASEOPERATIONS_API SQLiteDatabaseOperations : public IDatabaseOperations
{
public:
    SQLiteDatabaseOperations();
    int open(const std::string &fileName) override;
    int openForCreation(const std::string &fileName) override;
    int close() override;
    void *getConnectionPtr() const;
private:
	sqlite3 *db;
};