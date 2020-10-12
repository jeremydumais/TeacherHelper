#include "classStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;

struct FakeClassRow
{
    int id;
    string name;
    int schoolId;
    string schoolName;
    int cityId;
    string cityName;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, name, schoolId, schoolName, cityId, cityName }; 
    }
};

struct FakeClassStudentRow
{
    int classId;
    int studentId;
    string studentFirstName;
    string studentLastName;
    string studentComments;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { classId, studentId, studentFirstName, studentLastName, studentComments }; 
    }
};

struct FakeStudentRow
{
    int id;
    string firstName;
    string lastName;
    string comments;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, firstName, lastName, comments }; 
    }
    operator Student() const 
    { 
        return Student(id, firstName, lastName, comments); 
    }

};

FakeClassRow classSample1 { 1, "101", 1, "School1", 1, "New York" };
FakeClassRow classSample2 { 1, "201", 1, "School1", 1, "New York" };

FakeClassStudentRow classStudentSample1 { 1, 1, "Joe", "Blow", "" };
FakeClassStudentRow classStudentSample2 { 1, 2, "Jane", "Doe", "A comment" };

FakeStudentRow studentSample1 { 1, "Joe", "Blow", "" };
FakeStudentRow studentSample2 { 2, "Jane", "Doe", "A comment"};

TEST(ClassStorage_getAllItems, TwoClassesWithNoMembersNoError_ReturnListClasses)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { classSample1, classSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", {})
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(ClassStorage_getAllItems, TwoClassesWithTwoMembersNoError_ReturnListClasses)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { classSample1, classSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { classStudentSample1, classStudentSample2 })
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    auto actual { storage.getAllItems() };
    ASSERT_EQ(2, actual.size());
    ASSERT_EQ(2, actual.begin()->getMembers().size());
}

TEST(ClassStorage_getAllItems, ErrorLoadingClass_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(ClassStorage_getAllItems, ErrorLoadingMembers_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { classSample1, classSample2 }),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while doing the select operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
   
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while doing the select operation", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 1 } })
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass("MyClass", School("Test", City("New York")));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_insertItem, FailedInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "Error during the insert operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.insertItem(myClass));
    ASSERT_EQ("Error during the insert operation", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsertButFailedAtRetreivingRecordId_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(false, "Error while retreiving the id")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.insertItem(myClass));
    ASSERT_EQ("Error while retreiving the id", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsertButFailedAtRetreivingRecordIdNoRowReturned_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 1 } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", {})
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.insertItem(myClass));
    ASSERT_EQ(0, storage.retreiveAssignedRecordId());
    ASSERT_EQ("Unable to retreive the assigned id for the new record.", storage.getLastError());
}

TEST(ClassStorage_insertItem, ValidInsertAndAddingOneMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 1 } }),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_insertItem, ValidInsertAndAddingTwoMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 1 } }),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(Student(2, "Joe", "Blow"));
    myClass.addMember(Student(3, "Jane", "Dow"));
    ASSERT_TRUE(storage.insertItem(myClass));
}

TEST(ClassStorage_updateItem, ValidUpdateWithNoMembers_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {}),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtLoadMembers_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error during the update operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(ClassStorage_updateItem, FailedUpdateAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewUpdateResult(false, "Error during the update operation")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error during the update operation", storage.getLastError());
}

TEST(ClassStorage_updateItem, ValidUpdateWithOneRemovedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1 }),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, ValidUpdateWithTwoRemovedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1, studentSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtRemovingMember_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1 }),
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error at removing member")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error at removing member", storage.getLastError());
}

TEST(ClassStorage_updateItem, ValidUpdateWithOneAddedMember_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(studentSample1);

    ASSERT_TRUE(storage.updateItem(myClass));
}

TEST(ClassStorage_updateItem, FailedUpdateAtAddingMember_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewInsertResult(false, "Error at adding member")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));
    Class myClass(1, "MyClass", School("Test", City("New York")));
    myClass.addMember(studentSample1);

    ASSERT_FALSE(storage.updateItem(myClass));
    ASSERT_EQ("Error at adding member", storage.getLastError());
}

TEST(ClassStorage_deleteItem, ValidDeleteOfClassWithNoMembers_ReturnOK)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(ClassStorage_deleteItem, ValidDeleteOfClassWith2Members_ReturnOK)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1, studentSample2 }),
        FakeOperationResultFactory::createNewDeleteResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(1));
}

TEST(ClassStorage_deleteItem, FailedDeleteAtLoadingMembers_ReturnError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewSelectResult(false, "Error at loading members")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(1));
    ASSERT_EQ("Error at loading members", storage.getLastError());
}

TEST(ClassStorage_deleteItem, FailedDeleteAtRemovingMembers_ReturnError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1, studentSample2 }),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error at removing members", QueryResult::ERROR)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(1));
    ASSERT_EQ("Error at removing members", storage.getLastError());
}

TEST(ClassStorage_deleteItem, FailedDeleteAtRemovingClass_ReturnError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 0 } }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { studentSample1, studentSample2 }),
        FakeOperationResultFactory::createNewDeleteResult(true),
        FakeOperationResultFactory::createNewDeleteResult(false, "Error at removing members", QueryResult::ERROR)
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(1));
    ASSERT_EQ("Error at removing members", storage.getLastError());
}


TEST(ClassStorage_deleteItem, FailedDeleteByReferentialIntegrityContrainst_ReturnConstraintError)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 2 } })
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::CONSTRAINTERROR, storage.deleteItem(1));
}

TEST(ClassStorage_isReferentialIntegrityConstraint, FailedAtRetreiveReferentialIntegrity_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Error during fetching referential integrity constraint")
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.isReferentialIntegrityConstraint(1));
    ASSERT_EQ("Error during fetching referential integrity constraint", storage.getLastError());
}

TEST(ClassStorage_isReferentialIntegrityConstraint, FailedAtRetreiveReferentialIntegrityRow_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { })
    }) };
    ClassStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.isReferentialIntegrityConstraint(1));
    ASSERT_EQ("Unable to retreive the referential integrity constraints.", storage.getLastError());
}
