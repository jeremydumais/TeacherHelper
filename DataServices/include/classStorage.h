#pragma once

#include "databaseConnection.h"
#include "class.h"
#include "IStorageOperationFactory.h"
#include "ManagementItemStorageBase.h"
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

class CLASSSTORAGE_API ClassStorage : public ManagementItemStorageBase<Class>
{
public:
    explicit ClassStorage(const DatabaseConnection &connection, 
                          const std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr);
    std::string getSelectCommand() const override;
    Class getItemFromRecord(const IStorageSelectOperation &record) const override;
    void postGetStep(std::list<Class> &items) override;
    std::string getInsertCommand() const override;
    std::vector<std::string> getInsertValues(const Class &item) const override;
    bool postInsertStep(const Class &item) override;
    std::string getUpdateCommand() const override;
    std::vector<std::string> getUpdateValues(const Class &item) const override;
    bool preUpdateStep(const Class &item) override;
    bool postUpdateStep(const Class &item) override;
    std::string getDeleteCommand() const override;
    std::vector<std::string> getDeleteValues(size_t id) const override;
    QueryResult preDeleteStep(size_t id) override;
    std::string getReferentialIntegrityConstraintsCommand() const override;
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override;
private:
    std::list<Student> oldMembers;
    bool insertMembers(size_t classId, const std::vector<size_t> &studentIdToAdd);
    bool removeMembers(size_t classId,  const std::vector<size_t> &studentIdToRemove);
    std::multimap<size_t, Student> loadAllMembers();
    std::list<Student> loadClassMembers(size_t classId);
};