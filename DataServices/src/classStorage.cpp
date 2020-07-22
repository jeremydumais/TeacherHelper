#include "classStorage.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include <algorithm>
#include <iostream>
#include <sqlite3.h>
#include <string>

using namespace std;

ClassStorage::ClassStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Class> ClassStorage::getAllItems()
{
    int i =1;
    list<Class> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT class.id, class.name, school.id, school.name, city.id, city.name " 
        "FROM class "
        "INNER JOIN school ON school.id = class.school_id " 
        "INNER JOIN city ON city.id = school.city_id " 
        "WHERE class.deleted = 0 AND school.deleted=0 AND city.deleted = 0 ORDER BY class.name, school.name");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Class tempClass(sqlite3_column_int(stmt, 0),
                                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                School(
                                  sqlite3_column_int(stmt, 2),
                                  reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3))),
                                  City(
                                    sqlite3_column_int(stmt, 4),
                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 5))))));
            retVal.push_back(tempClass);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
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
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO class (name, school_id) VALUES(?, ?)",
        vector<string> { p_class.getName(), to_string(p_class.getSchool().getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
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
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE class SET name = ?, school_id = ? WHERE id = ?",
        vector<string> { p_class.getName(),
            to_string(p_class.getSchool().getId()),
            to_string(p_class.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
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

bool ClassStorage::deleteItem(size_t id)
{
    SQLiteUpdateOperation operation(*connection, 
        "UPDATE class SET deleted=1 WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

size_t ClassStorage::retreiveAssignedClassId()
{
    size_t classId {0};
    SQLiteSelectOperation operationSelectAssignedId(*connection, "SELECT last_insert_rowid()");
    if (operationSelectAssignedId.execute()) {
        sqlite3_stmt *stmt = operationSelectAssignedId.getStatement();
        int result = sqlite3_step(stmt);
        if (result == SQLITE_ROW ) {
            classId = sqlite3_column_int(stmt, 0);
        }
        else {
            lastError = "Unable to retreive the assigned id for the new class record.";
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operationSelectAssignedId.getLastError();
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
        SQLiteInsertOperation operationInsertMembers(*connection, sqlQueryInsertMembers, membersValues);
        if (!operationInsertMembers.execute()) {
            lastError = operationInsertMembers.getLastError();
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
    SQLiteUpdateOperation operation(*connection, sqlQueryRemoveMembers, membersValues);
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}


multimap<size_t, Student> ClassStorage::loadAllMembers() 
{
    multimap<size_t, Student> classStudents;
    SQLiteSelectOperation operationLoadMembers(*connection, 
        "SELECT class_id, student_id, student.firstname, student.lastname, student.comments "
        "FROM class_student "
        "INNER JOIN Student ON class_student.student_id = student.id");
    if (operationLoadMembers.execute()) {
        sqlite3_stmt *stmt = operationLoadMembers.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            classStudents.insert(make_pair(sqlite3_column_int(stmt, 0), 
                                            Student(sqlite3_column_int(stmt, 1),
                                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))),
                                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3))),
                                                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 4))))));
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        throw runtime_error(operationLoadMembers.getLastError());
    }
    return classStudents;
}

std::list<Student> ClassStorage::loadClassMembers(size_t classId)
{
    list<Student> retVal;
    SQLiteSelectOperation operationLoadMembers(*connection, 
        "SELECT student.id, student.firstname, student.lastname, student.comments "
        "FROM class_student "
        "INNER JOIN Student ON class_student.student_id = student.id "
        "WHERE class_student.class_id = ?", vector<string> { to_string(classId) });
    if (operationLoadMembers.execute()) {
        sqlite3_stmt *stmt = operationLoadMembers.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            retVal.emplace_back(Student(sqlite3_column_int(stmt, 0),
                                        reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                                        reinterpret_cast<const char *>((sqlite3_column_text(stmt, 2))),
                                        reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3)))));
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        throw runtime_error(operationLoadMembers.getLastError());
    }
    return retVal;
}
