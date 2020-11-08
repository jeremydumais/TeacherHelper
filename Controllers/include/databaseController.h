#pragma once

#include "version.h"
#include "IDatabaseConnection.h"
#include "IDatabaseController.h"
#include "IDatabaseManagementOperations.h"
#include "databaseVersionStorage.h"
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

class DATABASECONTROLLER_API DatabaseController : public IDatabaseController
{
public:
    explicit DatabaseController(std::unique_ptr<IDatabaseConnection> databaseConnection = nullptr,
                                std::unique_ptr<IDatabaseManagementOperations> databaseManagementOperations = nullptr,
                                std::unique_ptr<DatabaseVersionStorage> databaseVersionStorage = nullptr);
    bool isDatabaseOpened() const override;
    std::string getOpenedDatabaseName() const override;
    const IDatabaseConnection &getDatabaseConnection() const override;
    bool isDatabaseExist(const std::string &databaseName) const override;
    const std::string &getLastError() const override;
    boost::optional<Version> getVersion() override;
    bool isDatabaseUpgradeRequired() const override;
    void openDatabase(const std::string &databaseName) override;
    void closeDatabase() override;
    bool createDatabase(const std::string &databaseName) override;
private:
    std::string lastError;
    std::unique_ptr<IDatabaseConnection> databaseConnection;
    std::unique_ptr<IDatabaseManagementOperations> databaseManagementOperations;
    std::unique_ptr<DatabaseVersionStorage> databaseVersionStorage;
};