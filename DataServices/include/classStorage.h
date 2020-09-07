#pragma once

#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include "IStorageOperationFactory.h"
#include "class.h"
#include <list>
#include <map>
#include <memory>
#include <vector>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define CLASSSTORAGE_API __declspec(dllexport)   
    #else  
        #define CLASSSTORAGE_API __declspec(dllimport)   
    #endif
#else
    #define CLASSSTORAGE_API
#endif

class CLASSSTORAGE_API ClassStorage : public IManagementItemStorage<Class>
{
public:
    explicit ClassStorage(const DatabaseConnection &connection, 
                          const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::list<Class> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Class &p_class) override;
    bool updateItem(const Class &p_class) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
    size_t retreiveAssignedClassId();
    bool insertMembers(size_t classId, const std::vector<size_t> &studentIdToAdd);
    bool removeMembers(size_t classId,  const std::vector<size_t> &studentIdToRemove);
    std::multimap<size_t, Student> loadAllMembers();
    std::list<Student> loadClassMembers(size_t classId);
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};