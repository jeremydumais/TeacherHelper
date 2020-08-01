#pragma once

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

class DATABASECONNECTION_API DatabaseConnection
{
public:
    explicit DatabaseConnection(const std::string &dbName);
    virtual ~DatabaseConnection();
    const std::string getDbName() const;
    sqlite3 *getConnectionPtr() const;
    void open();
    void close();
    void create();
private:
    std::string dbName;
	sqlite3 *db;
};