#include "subjectStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

SubjectStorage::SubjectStorage(const IDatabaseConnection &connection, 
                               unique_ptr<IStorageOperationFactory> operationFactory)
    : ManagementItemStorageBase<Subject>(connection, move(operationFactory))
{
}

std::string SubjectStorage::getSelectCommand() const
{
    return "SELECT id, name, isdefault FROM subject ORDER BY name;";
}

Subject SubjectStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return Subject(record.getIntValue(0),
                   record.getStringValue(1),
                   record.getBoolValue(2));
}

std::string SubjectStorage::getInsertCommand() const
{
    return "INSERT INTO subject (name, isdefault) VALUES(?, ?)";
}

std::vector<std::string> SubjectStorage::getInsertValues(const Subject &item) const
{
    return { boost::trim_copy(item.getName()), item.getIsDefault() ? "1" : "0" };
}

bool SubjectStorage::preInsertStep(const Subject &item)
{
    //If isDefault is true and that there's already a default subject in the database, 
    //then put the old one to false and then proceed with the new one.
    if(item.getIsDefault()) {
        if (!updateAllRowsToRemoveDefault(item.getId())) {
            return false;
        }
    }
    return true;
}

std::string SubjectStorage::getUpdateCommand() const
{
    return "UPDATE subject SET name = ?, isdefault = ? WHERE id = ?";
}

std::vector<std::string> SubjectStorage::getUpdateValues(const Subject &item) const
{
    return { boost::trim_copy(item.getName()),
             item.getIsDefault() ? "1" : "0",
             to_string(item.getId()) };
}

bool SubjectStorage::preUpdateStep(const Subject &item)
{
    //If isDefault is true and that there's already a default subject in the database, 
    //then put the old one to false and then proceed with the new one.
    if(item.getIsDefault()) {
        if (!updateAllRowsToRemoveDefault(item.getId())) {
            return false;
        }
    }
    return true;
}

std::string SubjectStorage::getDeleteCommand() const
{
    return "DELETE FROM subject WHERE id = ?";
}

std::vector<std::string> SubjectStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

std::string SubjectStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT COUNT(id) FROM assessment WHERE subject_id = ?";
}

std::vector<std::string> SubjectStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id) };
}

bool SubjectStorage::updateAllRowsToRemoveDefault(size_t currentSubjectId)
{
    auto operationCheckDefault = operationFactory->createSelectOperation(*connection, 
        "SELECT COUNT() FROM subject WHERE isdefault=1 and id<>?",
    vector<string> { to_string(currentSubjectId) });
    if (operationCheckDefault->execute()) {
        if (operationCheckDefault->getRow() && operationCheckDefault->getIntValue(0) > 0) {
            auto operationUpdateDefault = operationFactory->createUpdateOperation(*connection, 
                "UPDATE subject SET isdefault = 0",
                vector<string>());
            if (!operationUpdateDefault->execute()) {
                lastError = operationUpdateDefault->getLastError();
                operationCheckDefault->close();
                return false;
            }
        }
        operationCheckDefault->close();
    }
    else {
        lastError = operationCheckDefault->getLastError();
        return false;
    }
    return true;
}
