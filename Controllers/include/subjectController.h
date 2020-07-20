#pragma once

#include "subject.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

class SubjectController
{
public:
    explicit SubjectController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<IManagementItemStorage<Subject>> managementItemStorage = nullptr);
    const std::list<Subject> &getSubjects() const;
    const Subject* findSubject(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
						 size_t currentlyEditedSubjectId) const;
    const std::string &getLastError() const;
    void loadSubjects();
    bool insertSubject(const Subject &subject);
    bool updateSubject(const Subject &subject);
    bool deleteSubject(size_t id);
private:
	std::list<Subject> subjects;
    std::unique_ptr<IManagementItemStorage<Subject>> storage;
    std::string lastError;
};