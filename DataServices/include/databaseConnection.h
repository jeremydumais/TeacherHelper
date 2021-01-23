#pragma once

#include "IDatabaseConnection.h"
#include "IDatabaseOperations.h"
#include "IFileSystemOperations.h"
#include "IStorageOperationFactory.h"
#include <memory>
#include <string>

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
    explicit DatabaseConnection(const std::string &dbName, 
                                std::unique_ptr<IFileSystemOperations> fileSystemOperations = nullptr,
                                std::unique_ptr<IDatabaseOperations> databaseOperations = nullptr,
                                std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    virtual ~DatabaseConnection();
    const std::string &getDbName() const override;
    void *getConnectionPtr() const override;
    void open() override;
    void openForCreation() override;
    bool isOpened() const override;
    void close() override;
private:
    std::string dbName;
    bool isDBOpened;
    std::unique_ptr<IFileSystemOperations> fileSystemOperations;
    std::unique_ptr<IDatabaseOperations> databaseOperations;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};