#include "classStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include <algorithm>
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

ClassStorage::ClassStorage(const DatabaseConnection &connection, 
                           unique_ptr<IStorageOperationFactory> operationFactory)
    : connection(&connection),
      lastError(""),
      operationFactory { operationFactory ? 
                         move(operationFactory) : 
                         make_unique<SQLiteOperationFactory>()}
{
}

list<Class> ClassStorage::getAllItems()
{
    int i =1;
    list<Class> retVal;
    auto operation = operationFactory->createSelectOperation(*connection, 
        "SELECT class.id, class.name, school.id, school.name, city.id, city.name " 
        "FROM class "
        "INNER JOIN school ON school.id = class.school_id " 
        "INNER JOIN city ON city.id = school.city_id " 
        "ORDER BY class.name, school.name");
    if (operation->execute()) {
        while (operation->getRow()) {
            Class tempClass(operation->getIntValue(0),
                                operation->getStringValue(1),
                                School(
                                  operation->getIntValue(2),
                                  operation->getStringValue(3),
                                  City(
                                    operation->getIntValue(4),
                                    operation->getStringValue(5))));
            retVal.push_back(tempClass);
        }
        operation->close();
    }
    else {
        lastError = operation->getLastError();
        return list<Class>();
    }
    //Load all members
    multimap<size_t, Student> classStudents;
    try {
        classStudents = loadAllMembers();
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return list<Class>();
    }
    //Associate all members to classes
    for(auto &aClass : retVal) {
        auto it = classStudents.find(aClass.getId());
        while(it != classStudents.end() && it->first == aClass.getId()) {
            aClass.addMember(it->second);
            it++;
        }
    }
    return retVal;
}

const std::string &ClassStorage::getLastError() const
{
    return lastError;
}

bool ClassStorage::insertItem(const Class &p_class)
{
    auto operation = operationFactory->createInsertOperation(*connection, 
        "INSERT INTO class (name, school_id) VALUES(?, ?)",
        vector<string> { p_class.getName(), to_string(p_class.getSchool().getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    //Retreive the assigned primary key (id)
    size_t classId = retreiveAssignedClassId();
    if (classId == 0) {
        return false;
    }

    vector<size_t> memberIdsToAdd;
    transform(p_class.getMembers().begin(), p_class.getMembers().end(), std::back_inserter(memberIdsToAdd),
               [](Student const& x) { return x.getId(); });
    return insertMembers(classId, memberIdsToAdd);
}

bool ClassStorage::updateItem(const Class &p_class)
{
    //Load old members
    list<Student> oldMembers;
    try {
        oldMembers = loadClassMembers(p_class.getId());
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return false;
    }
    //Update current class
    auto operation = operationFactory->createUpdateOperation(*connection, 
        "UPDATE class SET name = ?, school_id = ? WHERE id = ?",
        vector<string> { p_class.getName(),
            to_string(p_class.getSchool().getId()),
            to_string(p_class.getId()) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    //Remove old members
    vector<size_t> studentIdsToRemove;
    for(const auto &oldMember : oldMembers) {
        if (find(p_class.getMembers().begin(), p_class.getMembers().end(), oldMember) == p_class.getMembers().end()) {
            studentIdsToRemove.emplace_back(oldMember.getId());
        }
    }
    if (studentIdsToRemove.size() > 0 && !removeMembers(p_class.getId(), studentIdsToRemove)) {
        return false;
    }
    //Add new members
    vector<size_t> studentIdsToAdd;
    for(const auto &member : p_class.getMembers()) {
        if (find(oldMembers.begin(), oldMembers.end(), member) == oldMembers.end()) {
            studentIdsToAdd.emplace_back(member.getId());
        }
    }
    return insertMembers(p_class.getId(), studentIdsToAdd);
}

QueryResult ClassStorage::deleteItem(size_t id)
{
    //Load old members
    list<Student> oldMembers;
    try {
        oldMembers = loadClassMembers(id);
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return QueryResult::ERROR;
    }
    //Delete all the members of the class
    vector<size_t> memberIdsToRemove;
    transform(oldMembers.begin(), oldMembers.end(), std::back_inserter(memberIdsToRemove),
               [](Student const& x) { return x.getId(); });
    if (memberIdsToRemove.size() > 0 && !removeMembers(id, memberIdsToRemove)) {
        return QueryResult::ERROR;
    }
    //Delete the class
    auto operation = operationFactory->createDeleteOperation(*connection, 
        "DELETE FROM class WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation->execute()) {
        lastError = operation->getLastError();
    }
    return operation->getExtendedResultInfo();
}

size_t ClassStorage::retreiveAssignedClassId()
{
    size_t classId {0};
    auto operationSelectAssignedId = operationFactory->createSelectOperation(*connection, 
     "SELECT last_insert_rowid()");
    if (operationSelectAssignedId->execute()) {
        if (operationSelectAssignedId->getRow()) {
            classId = operationSelectAssignedId->getIntValue(0);
        }
        else {
            lastError = "Unable to retreive the assigned id for the new class record.";
        }
        operationSelectAssignedId->close();
    }
    else {
        lastError = operationSelectAssignedId->getLastError();
    }
    return classId;
}

bool ClassStorage::insertMembers(size_t classId, const std::vector<size_t> &studentIdToAdd)
{
    if (studentIdToAdd.size() > 0) {
        //Insert all members
        string sqlQueryInsertMembers {"INSERT INTO class_student (class_id, student_id) VALUES" };
        vector<string> membersValues;
        size_t i {0};
        for (const auto &memberId : studentIdToAdd) {
            sqlQueryInsertMembers += " (?, ?)";
            membersValues.emplace_back(to_string(classId));
            membersValues.emplace_back(to_string(memberId));
            i++;
            if (i < studentIdToAdd.size()) {
                sqlQueryInsertMembers += ",";
            }
        }
        auto operationInsertMembers = operationFactory->createInsertOperation(*connection, sqlQueryInsertMembers, membersValues);
        if (!operationInsertMembers->execute()) {
            lastError = operationInsertMembers->getLastError();
            return false;
        }
    }
    return true;
}

bool ClassStorage::removeMembers(size_t classId, const vector<size_t> &studentIdToRemove)
{
    string sqlQueryRemoveMembers {"DELETE FROM class_student WHERE" };
    vector<string> membersValues;
    for (size_t i = 0; i < studentIdToRemove.size(); i++)
    {
        if (i > 0) {
           sqlQueryRemoveMembers += " OR";
        }
        sqlQueryRemoveMembers += " (class_id = ? AND student_id = ?)";
        membersValues.emplace_back(to_string(classId));
        membersValues.emplace_back(to_string(studentIdToRemove[i]));
    }
    auto operation = operationFactory->createDeleteOperation(*connection, sqlQueryRemoveMembers, membersValues);
    if (!operation->execute()) {
        lastError = operation->getLastError();
        return false;
    }
    return true;
}


multimap<size_t, Student> ClassStorage::loadAllMembers() 
{
    multimap<size_t, Student> classStudents;
    auto operationLoadMembers = operationFactory->createSelectOperation(*connection, 
        "SELECT class_id, student_id, student.firstname, student.lastname, student.comments "
        "FROM class_student "
        "INNER JOIN Student ON class_student.student_id = student.id");
    if (operationLoadMembers->execute()) {
        while (operationLoadMembers->getRow()) {
            classStudents.insert(make_pair(operationLoadMembers->getIntValue(0), 
                                            Student(operationLoadMembers->getIntValue(1),
                                                    operationLoadMembers->getStringValue(2),
                                                    operationLoadMembers->getStringValue(3),
                                                    operationLoadMembers->getStringValue(4))));
        }
        operationLoadMembers->close();
    }
    else {
        throw runtime_error(operationLoadMembers->getLastError());
    }
    return classStudents;
}

std::list<Student> ClassStorage::loadClassMembers(size_t classId)
{
    list<Student> retVal;
    auto operationLoadMembers = operationFactory->createSelectOperation(*connection, 
        "SELECT student.id, student.firstname, student.lastname, student.comments "
        "FROM class_student "
        "INNER JOIN Student ON class_student.student_id = student.id "
        "WHERE class_student.class_id = ?", vector<string> { to_string(classId) });
    if (operationLoadMembers->execute()) {
        while (operationLoadMembers->getRow()) {
            retVal.emplace_back(operationLoadMembers->getIntValue(0),
                                        operationLoadMembers->getStringValue(1),
                                        operationLoadMembers->getStringValue(2),
                                        operationLoadMembers->getStringValue(3));
        }
        operationLoadMembers->close();
    }
    else {
        throw runtime_error(operationLoadMembers->getLastError());
    }
    return retVal;
}
