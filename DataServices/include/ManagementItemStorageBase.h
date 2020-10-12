#pragma once

#include "databaseConnection.h"
#include "IStorageOperationFactory.h"
#include "sqliteOperationFactory.h"
#include "queryResult.h"
#include <list>
#include <memory>
#include <stdexcept>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define MANAGEMENTITEMSTORAGEBASE_API __declspec(dllexport)   
    #else  
        #define MANAGEMENTITEMSTORAGEBASE_API __declspec(dllimport)   
    #endif
#else
    #define MANAGEMENTITEMSTORAGEBASE_API
#endif

template <class T>
class MANAGEMENTITEMSTORAGEBASE_API ManagementItemStorageBase
{
public:
    explicit ManagementItemStorageBase(const DatabaseConnection &connection, 
                         std::unique_ptr<IStorageOperationFactory> operationFactory = nullptr)
        : connection(&connection),
        lastError(""),
        operationFactory { operationFactory ? 
                           move(operationFactory) :  
                           std::make_unique<SQLiteOperationFactory>()}
    {}
    virtual ~ManagementItemStorageBase() {};
    
    virtual std::list<T> getAllItems() {
        std::list<T> retVal;
        auto operation = operationFactory->createSelectOperation(*connection, 
            getSelectCommand());
        if (operation->execute()) {
            while (operation->getRow()) {
                retVal.push_back(getItemFromRecord(*operation.get()));
            }
            operation->close();
        }
        else {
            lastError = operation->getLastError();
            return retVal;
        }
        try {
            postGetStep(retVal);
        }
        catch(std::runtime_error &err) {
            lastError = err.what();
            return std::list<T>();
        }
        return retVal;
    }

    virtual void postGetStep(std::list<T> &items) {
    }
    
    virtual const std::string &getLastError() const {
        return lastError;
    }

    virtual bool insertItem(const T &item) {
        //Optional pre-insertStep
        if (!preInsertStep(item)) {
            return false;
        }
        auto operation = operationFactory->createInsertOperation(*connection, 
            getInsertCommand(),
            getInsertValues(item));
        if (!operation->execute()) {
            lastError = operation->getLastError();
            return false;
        }
        //Optional post-insertStep
        if (!postInsertStep(item)) {
            return false;
        }
        return true;
    }

    virtual bool preInsertStep(const T &item) { return true; }
    virtual bool postInsertStep(const T &item) { return true; }

    virtual bool updateItem(const T &item) {
        //Optional pre-updateStep
        if (!preUpdateStep(item)) {
            return false;
        }
        auto operation = operationFactory->createUpdateOperation(*connection, 
            getUpdateCommand(),
            getUpdateValues(item));
        if (!operation->execute()) {
            lastError = operation->getLastError();
            return false;
        }
        //Optional post-updateStep
        if (!postUpdateStep(item)) {
            return false;
        }
        return true;
    }
        
    virtual bool preUpdateStep(const T &item) { return true; }
    virtual bool postUpdateStep(const T &item) { return true; }

    virtual QueryResult deleteItem(size_t id) {
        //Ensure that the record is not used as a foreign key in another table
        if (isReferentialIntegrityConstraint(id)) {
            return QueryResult::CONSTRAINTERROR;
        }
        //Optional pre-deleteStep
        auto preDeleteStepResult = preDeleteStep(id);
        if (preDeleteStepResult != QueryResult::OK) {
            return preDeleteStepResult;
        }
        auto operation = operationFactory->createDeleteOperation(*connection, 
            getDeleteCommand(), 
            getDeleteValues(id));
        if (!operation->execute()) {
            lastError = operation->getLastError();
        }
        //Optional post-deleteStep
        auto postDeleteStepResult = postDeleteStep(id);
        if (postDeleteStepResult != QueryResult::OK) {
            return postDeleteStepResult;
        } 
        else {
            return operation->getExtendedResultInfo();
        }
    }

    virtual QueryResult preDeleteStep(size_t id) { return QueryResult::OK; }
    virtual QueryResult postDeleteStep(size_t id) { return QueryResult::OK; }

    virtual bool isReferentialIntegrityConstraint(size_t id) {
        size_t nbOfConstraints {0};
        auto operationSelectConstraints = operationFactory->createSelectOperation(*connection, 
            getReferentialIntegrityConstraintsCommand(),
            getReferentialIntegrityConstraintsValues(id));
        if (operationSelectConstraints->execute()) {
            if (operationSelectConstraints->getRow()) {
                nbOfConstraints = operationSelectConstraints->getIntValue(0);
            }
            else {
                lastError = "Unable to retreive the referential integrity constraints.";
            }
            operationSelectConstraints->close();
        }
        else {
            lastError = operationSelectConstraints->getLastError();
        }
        return nbOfConstraints > 0;
    }

    virtual size_t retreiveAssignedRecordId()
    {
        size_t recordId {0};
        auto operationSelectAssignedId = operationFactory->createSelectOperation(*connection, 
        "SELECT last_insert_rowid()");
        if (operationSelectAssignedId->execute()) {
            if (operationSelectAssignedId->getRow()) {
                recordId = operationSelectAssignedId->getIntValue(0);
            }
            else {
                lastError = "Unable to retreive the assigned id for the new record.";
            }
            operationSelectAssignedId->close();
        }
        else {
            lastError = operationSelectAssignedId->getLastError();
        }
        return recordId;
    }

protected:
    virtual std::string getSelectCommand() const = 0;
    virtual T getItemFromRecord(const IStorageSelectOperation &record) const = 0;
    virtual std::string getInsertCommand() const = 0;
    virtual std::vector<std::string> getInsertValues(const T &item) const = 0;
    virtual std::string getUpdateCommand() const = 0;
    virtual std::vector<std::string> getUpdateValues(const T &item) const = 0;
    virtual std::string getDeleteCommand() const = 0;
    virtual std::vector<std::string> getDeleteValues(size_t id) const = 0;
    virtual std::string getReferentialIntegrityConstraintsCommand() const = 0;
    virtual std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const = 0;
protected:
    const DatabaseConnection * const connection;
    std::string lastError;
    std::unique_ptr<IStorageOperationFactory> operationFactory;
};