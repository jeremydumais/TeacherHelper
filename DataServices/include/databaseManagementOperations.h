#pragma once

#include "databaseVersionStorage.h"
#include "IDatabaseConnectionFactory.h"
#include "IDatabaseOperations.h"
#include "IDatabaseManagementOperations.h"
#include "IFileSystemOperations.h"
#include "IStorageOperationFactory.h"
#include <memory>
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
    explicit DatabaseManagementOperations(std::shared_ptr<IFileSystemOperations> fileSystemOperations = nullptr,
                                          std::shared_ptr<IDatabaseOperations> databaseOperations = nullptr,
                                          std::unique_ptr<IDatabaseConnectionFactory> connectionFactory = nullptr,
                                          std::shared_ptr<IStorageOperationFactory> operationFactory = nullptr);
    const std::string &getLastError() const override;
    bool exist(const std::string &databaseName) const override;
    bool create(const std::string &databaseName) override; 
    bool upgrade(IDatabaseConnection &connection) override;
private:
    std::string lastError;
    std::shared_ptr<IFileSystemOperations> fileSystemOperations;
    std::shared_ptr<IDatabaseOperations> databaseOperations;
    std::unique_ptr<IDatabaseConnectionFactory> connectionFactory;
    std::shared_ptr<IStorageOperationFactory> operationFactory;
    std::string generateBackupDatabaseFileName() const;
    bool migrateToV1_1_0(IDatabaseConnection &connection, DatabaseVersionStorage &databaseVersionStorage);
};