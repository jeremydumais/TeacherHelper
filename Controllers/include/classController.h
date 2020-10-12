#pragma once

#include "class.h"
#include "ManagementItemStorageBase.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define CLASSCONTROLLER_API __declspec(dllexport)   
    #else  
        #define CLASSCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define CLASSCONTROLLER_API
#endif

class CLASSCONTROLLER_API ClassController
{
public:
    explicit ClassController(const DatabaseConnection &dbConnection,
                            std::unique_ptr<ManagementItemStorageBase<Class>> managementItemStorage = nullptr);
    const std::list<Class> &getClasses() const;
    const std::list<Class> getClassesBySchool(const School &school) const;
    const Class* findClass(size_t id) const;
    bool isNewNameAvailableForAdd(const std::string &name, const size_t school_id) const;
	bool isNewNameAvailableForUpdate(const std::string &name, 
                         const size_t school_id,
						 size_t currentlyEditedClassId) const;
    const std::string &getLastError() const;
    void loadClasses();
    bool insertClass(const Class &p_class);
    bool updateClass(const Class &p_class);
    bool deleteClass(size_t id);
private:
	std::list<Class> classes;
    std::unique_ptr<ManagementItemStorageBase<Class>> storage;
    std::string lastError;
};