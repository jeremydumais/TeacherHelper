#include "assessmentStorage.h"
#include "sqliteDateTimeFactory.h"
#include "sqliteInsertOperation.h"
#include "sqliteSelectOperation.h"
#include "sqliteUpdateOperation.h"
#include "sqliteDeleteOperation.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>

using namespace std;

AssessmentStorage::AssessmentStorage(const DatabaseConnection &connection)
    : connection(&connection),
      lastError("")
{
}

list<Assessment> AssessmentStorage::getAllItems()
{
    return loadItemsFromDB();
}

list<Assessment> AssessmentStorage::getItemsByClassId(const size_t classId) 
{
    return loadItemsFromDB(fmt::format("WHERE class.id = {0} ", classId));
}

list<Assessment> AssessmentStorage::loadItemsFromDB(const string &whereClause) 
{
    list<Assessment> retVal;
    SQLiteSelectOperation operation(*connection, 
        "SELECT assessment.id, assessment.name, testType.id, testType.name, subject.id, subject.name, " 
        "  class.id, class.name, school.id, school.name, city.id, city.name, date "
        "FROM assessment "
        "INNER JOIN testType ON testType.id = assessment.testType_id " 
        "INNER JOIN subject ON subject.id = assessment.subject_id " 
        "INNER JOIN class ON class.id = assessment.class_id " 
        "INNER JOIN school ON school.id = class.school_id " 
        "INNER JOIN city ON city.id = school.city_id " +
        whereClause +
        " ORDER BY datetime(date), assessment.name");
    if (operation.execute()) {
        sqlite3_stmt *stmt = operation.getStatement();
        int result = sqlite3_step(stmt);
        while (result == SQLITE_ROW) {
            Assessment tempAssessment(sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char *>((sqlite3_column_text(stmt, 1))),
                TestType(
                    sqlite3_column_int(stmt, 2),
                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 3)))),
                Subject(
                    sqlite3_column_int(stmt, 4),
                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 5)))),
                Class(
                    sqlite3_column_int(stmt, 6),
                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 7))),
                    School(
                    sqlite3_column_int(stmt, 8),
                    reinterpret_cast<const char *>((sqlite3_column_text(stmt, 9))),
                    City(
                        sqlite3_column_int(stmt, 10),
                        reinterpret_cast<const char *>((sqlite3_column_text(stmt, 11)))))),
                SQLiteDateTimeFactory::NewDateTimeFromISOExtended(reinterpret_cast<const char *>((sqlite3_column_text(stmt, 12)))).getBoostPTime());
            retVal.push_back(tempAssessment);
            result = sqlite3_step(stmt);
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operation.getLastError();
        return list<Assessment>();
    }
    //Load all members
   /* multimap<size_t, Student> classStudents;
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
    }*/
    return retVal;
}

const string &AssessmentStorage::getLastError() const
{
    return lastError;
}

bool AssessmentStorage::insertItem(const Assessment &assessment)
{
    auto assessmentDateTime = SQLiteDateTimeFactory::NewDateTimeFromPTime(assessment.getDateTime());
    SQLiteInsertOperation operation(*connection, 
        "INSERT INTO assessment (name, testType_id, subject_id, class_id, date) "
        "VALUES(?, ?, ?, ?, ?)",
        vector<string> { boost::trim_copy(assessment.getName()),
                         to_string(assessment.getTestType().getId()), 
                         to_string(assessment.getSubject().getId()), 
                         to_string(assessment.getClass().getId()), 
                         assessmentDateTime.toSQLiteString()
                       });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }
    return true;
}

size_t AssessmentStorage::retreiveAssignedAssessmentId()
{
    size_t assessmentId {0};
    SQLiteSelectOperation operationSelectAssignedId(*connection, "SELECT last_insert_rowid()");
    if (operationSelectAssignedId.execute()) {
        sqlite3_stmt *stmt = operationSelectAssignedId.getStatement();
        int result = sqlite3_step(stmt);
        if (result == SQLITE_ROW ) {
            assessmentId = sqlite3_column_int(stmt, 0);
        }
        else {
            lastError = "Unable to retreive the assigned id for the new assessment record.";
        }
        sqlite3_finalize(stmt);
    }
    else {
        lastError = operationSelectAssignedId.getLastError();
    }
    return assessmentId;
}

bool AssessmentStorage::updateItem(const Assessment &assessment)
{
    /*SQLiteUpdateOperation operation(*connection, 
        "UPDATE assessment SET name = ? WHERE id = ?",
        vector<string> { boost::trim_copy(city.getName()),
            to_string(city.getId()) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
        return false;
    }*/
    return true;
}

QueryResult AssessmentStorage::deleteItem(size_t id)
{
    //Ensure the the record is not user as a foreign key in another table

    /*SQLiteDeleteOperation operation(*connection, 
        "DELETE FROM city WHERE id = ?", 
        vector<string> { to_string(id) });
    if (!operation.execute()) {
        lastError = operation.getLastError();
    }
    return operation.getExtendedResultInfo();*/
    return QueryResult::ERROR;
}
