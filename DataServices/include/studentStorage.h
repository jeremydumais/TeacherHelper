#pragma once

#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "IStorageOperationFactory.h"
#include "student.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define STUDENTSTORAGE_API __declspec(dllexport)   
    #else  
        #define STUDENTSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define STUDENTSTORAGE_API
#endif

class STUDENTSTORAGE_API StudentStorage : public IManagementItemStorage<Student>
{
public:
    explicit StudentStorage(const DatabaseConnection &connection, 
                            const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::list<Student> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Student &student) override;
    bool updateItem(const Student &student) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};