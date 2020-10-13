#pragma once

#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
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
    explicit DatabaseVersionStorage(const DatabaseConnection &connection, 
                                    const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
};