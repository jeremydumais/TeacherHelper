#pragma once

#include "IDatabaseConnection.h"
#include <string>
#include <sqlite3.h>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define DATABASECONNECTION_API __declspec(dllexport)   
    #else  
        #define DATABASECONNECTION_API __declspec(dllimport)   
    #endif
#else
    #define DATABASECONNECTION_API
#endif

class DATABASECONNECTION_API DatabaseConnection : public IDatabaseConnection
{
public:
    explicit DatabaseConnection(const std::string &dbName);
    virtual ~DatabaseConnection();
    const std::string &getDbName() const override;
    sqlite3 *getConnectionPtr() const;
    void open() override;
    bool isOpened() const override;
    void close() override;
    void create();
private:
    std::string dbName;
    bool isDBOpened;
	sqlite3 *db;
};