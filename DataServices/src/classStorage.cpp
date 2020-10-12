#include "classStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include "sqliteOperationFactory.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

ClassStorage::ClassStorage(const DatabaseConnection &connection, 
                           unique_ptr<IStorageOperationFactory> operationFactory)
    : ManagementItemStorageBase<Class>(connection, move(operationFactory)),
      oldMembers(list<Student>())
{
}

std::string ClassStorage::getSelectCommand() const
{
    return "SELECT class.id, class.name, school.id, school.name, city.id, city.name " 
           "FROM class "
           "INNER JOIN school ON school.id = class.school_id " 
           "INNER JOIN city ON city.id = school.city_id " 
           "ORDER BY class.name, school.name";
}

Class ClassStorage::getItemFromRecord(const IStorageSelectOperation &record) const
{
    return Class(record.getIntValue(0),
                 record.getStringValue(1),
                 School(record.getIntValue(2),
                        record.getStringValue(3),
                        City(record.getIntValue(4),
                             record.getStringValue(5))));
}

void ClassStorage::postGetStep(std::list<Class> &items) 
{
    auto classStudents = loadAllMembers();
    //Associate all members to classes
    for(auto &aClass : items) {
        auto it = classStudents.find(aClass.getId());
        while(it != classStudents.end() && it->first == aClass.getId()) {
            aClass.addMember(it->second);
            it++;
        }
    }
}

std::string ClassStorage::getInsertCommand() const
{
    return "INSERT INTO class (name, school_id) VALUES(?, ?)";
}

std::vector<std::string> ClassStorage::getInsertValues(const Class &item) const
{
    return { item.getName(), to_string(item.getSchool().getId()) };
}

bool ClassStorage::postInsertStep(const Class &item) 
{
    //Retreive the assigned primary key (id)
    size_t classId = retreiveAssignedRecordId();
    if (classId == 0) {
        return false;
    }
    //Add all students to the class members
    vector<size_t> memberIdsToAdd;
    transform(item.getMembers().begin(), item.getMembers().end(), std::back_inserter(memberIdsToAdd),
               [](Student const& x) { return x.getId(); });
    return insertMembers(classId, memberIdsToAdd);
}

std::string ClassStorage::getUpdateCommand() const
{
    return "UPDATE class SET name = ?, school_id = ? WHERE id = ?";
}

std::vector<std::string> ClassStorage::getUpdateValues(const Class &item) const
{
    return { item.getName(),
             to_string(item.getSchool().getId()),
             to_string(item.getId()) };
}

bool ClassStorage::preUpdateStep(const Class &item) 
{ 
    //Load old members
    oldMembers = list<Student>();
    try {
        oldMembers = loadClassMembers(item.getId());
    }
    catch(runtime_error &err) {
        lastError = err.what();
        return false;
    }
    return true;
}

bool ClassStorage::postUpdateStep(const Class &item) 
{
    //Remove old members
    vector<size_t> studentIdsToRemove;
    for(const auto &oldMember : oldMembers) {
        if (find(item.getMembers().begin(), item.getMembers().end(), oldMember) == item.getMembers().end()) {
            studentIdsToRemove.emplace_back(oldMember.getId());
        }
    }
    if (studentIdsToRemove.size() > 0 && !removeMembers(item.getId(), studentIdsToRemove)) {
        return false;
    }
    //Add new members
    vector<size_t> studentIdsToAdd;
    for(const auto &member : item.getMembers()) {
        if (find(oldMembers.begin(), oldMembers.end(), member) == oldMembers.end()) {
            studentIdsToAdd.emplace_back(member.getId());
        }
    }
    return insertMembers(item.getId(), studentIdsToAdd);
}

std::string ClassStorage::getDeleteCommand() const
{
    return "DELETE FROM class WHERE id = ?";
}

std::vector<std::string> ClassStorage::getDeleteValues(size_t id) const
{   
    return { to_string(id) };
}

QueryResult ClassStorage::preDeleteStep(size_t id) 
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
    return QueryResult::OK;
}

std::string ClassStorage::getReferentialIntegrityConstraintsCommand() const
{
    return "SELECT COUNT(id) FROM assessment WHERE class_id = ?";
}

std::vector<std::string> ClassStorage::getReferentialIntegrityConstraintsValues(size_t id) const
{
    return { to_string(id) };
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
