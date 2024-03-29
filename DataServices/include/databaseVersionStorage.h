#pragma once

#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "version.h"
#include <boost/optional.hpp>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define DATABASEVERSIONSTORAGE_API __declspec(dllexport)   
    #else  
        #define DATABASEVERSIONSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define DATABASEVERSIONSTORAGE_API
#endif

class DATABASEVERSIONSTORAGE_API DatabaseVersionStorage
{
public:
    explicit DatabaseVersionStorage(const IDatabaseConnection &connection, 
                                    const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    boost::optional<Version> getVersion();
    const std::string &getLastError() const;
    bool updateVersion(Version version);
protected:
    const IDatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
private:
    bool isVersionTableExist();
};