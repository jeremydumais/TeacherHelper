#pragma once

#include "subject.h"
#include "databaseConnection.h"
#include "IManagementItemStorage.h"
#include <list>

class SubjectStorage : public IManagementItemStorage<Subject>
{
public:
    explicit SubjectStorage(const DatabaseConnection &connection);
    std::list<Subject> getAllItems() override;
    const std::string &getLastError() const override;
    bool insertItem(const Subject &subject) override;
    bool updateItem(const Subject &subject) override;
    QueryResult deleteItem(size_t id) override;
private:
    const DatabaseConnection * const connection;
    std::string lastError;
    bool updateAllRowsToRemoveDefault(size_t currentSubjectId);
};