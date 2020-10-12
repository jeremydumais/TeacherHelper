#include "assessmentStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include "sqliteDateTimeFactory.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

struct FakeAssessmentRow
{
    int id;
    string name;
    int testTypeId;
    string testTypeName;
    int subjectId;
    string subjectName;
    int classId;
    string className;
    int schoolId;
    string schoolName;
    int cityId;
    string cityName;
    string date;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id,
                                    name,
                                    testTypeId,
                                    testTypeName,
                                    subjectId,
                                    subjectName,
                                    classId,
                                    className,
                                    schoolId,
                                    schoolName,
                                    cityId,
                                    cityName,
                                    date }; 
    }
    operator Assessment() const 
    { 
        return Assessment(id, name, 
                          TestType(testTypeId, testTypeName), 
                          Subject(subjectId, subjectName),
                          Class(classId, className, School(schoolId, schoolName, City(cityId, cityName))),
                          SQLiteDateTimeFactory::NewDateTimeFromISOExtended(date).getBoostPTime()); 
    }
};

struct FakeAssessmentResultRow
{
    int assessmentId;
    int id;
    double result;
    string comments;
    int studentId;
    string studentFirstName;
    string studentLastName;
    string studentComments;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { assessmentId,
                                    id,
                                    result,
                                    comments,
                                    studentId,
                                    studentFirstName,
                                    studentLastName,
                                    studentComments }; 
    }
    operator AssessmentResult() const 
    { 
        return AssessmentResult(id, 
                                Student(studentId, studentFirstName, studentLastName, studentComments),
                                result, comments); 
    }
};

FakeAssessmentRow assessmentSample1 { 1, "Intra Exam", 
                                      1, "Exam", 
                                      1, "History",
                                      1, "MyClass",
                                      1, "SchoolTest",
                                      1, "CityTest",
                                      "2020-08-23T13:21:33" };

FakeAssessmentRow assessmentSample2 { 2, "Final Exam", 
                                      2, "FinalExam", 
                                      2, "Art",
                                      1, "MyClass",
                                      1, "SchoolTest",
                                      1, "CityTest",
                                      "2020-08-22T11:11:13" };

FakeAssessmentResultRow assessmentResultSample1 { 1, 1, 90.5f, "", 1, "Joe", "Blow", ""};
FakeAssessmentResultRow assessmentResultSample2 { 1, 2, 87.2f, "A result comment", 2, "Jane", "Doe", ""};

class AssessmentStorageWithSampleTwoResult : public ::testing::Test
{
public:
	AssessmentStorageWithSampleTwoResult()
	  : assessmentSample(assessmentSample1),
		result1 { assessmentResultSample1 },
		result2 { assessmentResultSample2 }
	{
		assessmentSample.addResult(result1);
		assessmentSample.addResult(result2);
	}
	Assessment assessmentSample;
	AssessmentResult result1;
	AssessmentResult result2;
};

TEST(AssessmentStorage_getAllItems, TwoAssessmentsWithNoResultsNoError_ReturnListAssessments)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1, assessmentSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(AssessmentStorage_getAllItems, TwoAssessmentsWithOneResultsNoError_ReturnListAssessments)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1, assessmentSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1 })
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    auto items { storage.getAllItems() };
    ASSERT_EQ(2, items.size());
    ASSERT_EQ(1, items.begin()->getResults().size());
}

TEST(AssessmentStorage_getAllItems, ErrorAtFirstSelect_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while loading the assessments."),
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while loading the assessments.", storage.getLastError());
}

TEST(AssessmentStorage_getAllItems, ErrorAtSecondSelect_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1, assessmentSample2 }),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while loading the results."),
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while loading the results.", storage.getLastError());
}

TEST(AssessmentStorage_getItemsByClassId, NoAssessmentForClass_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1, assessmentSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1 })
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    auto items { storage.getItemsByClassId(1) };
    ASSERT_EQ(2, items.size());
    ASSERT_EQ(1, items.begin()->getResults().size());
}

TEST(AssessmentStorage_getItemById, WithReturningTwoAssessment_ReturnNoAssessment)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1, assessmentSample2 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1 })
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    auto items { storage.getItemById(1) };
    ASSERT_FALSE(items);
    ASSERT_EQ("More than one assessment was returned.", storage.getLastError());
}

TEST(AssessmentStorage_getItemById, WithReturningNoAssessment_ReturnNoAssessment)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", {}),
        FakeOperationResultFactory::createNewSelectResult(true, "", {})
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    auto items { storage.getItemById(1) };
    ASSERT_FALSE(items);
    ASSERT_EQ("No assessment was returned.", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertItem_ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 3 } }),
        FakeOperationResultFactory::createNewInsertResult(true)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertItem_ErrorAtInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "An error occurred while inserting")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(assessmentSample));
    ASSERT_EQ("An error occurred while inserting", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertItem_ErrorAtRetreiveId_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while retreiving id")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertItem(assessmentSample));
    ASSERT_EQ("An error occurred while retreiving id", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, retreiveAssignedAssessmentId_ValidInsert_ReturnId)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 3 } }),
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 3 } })
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
    ASSERT_EQ(3, storage.retreiveAssignedRecordId());
}

TEST_F(AssessmentStorageWithSampleTwoResult, retreiveAssignedAssessmentId_ErrorAtSelect_Return0)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 3 } }),
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while selecting")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
    ASSERT_EQ(0, storage.retreiveAssignedRecordId());
    ASSERT_EQ("An error occurred while selecting", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, retreiveAssignedAssessmentId_ErrorAtSelectNoRowReturned_Return0)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", { vector<boost::any> { 3 } }),
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", {})
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
    ASSERT_EQ(0, storage.retreiveAssignedRecordId());
    ASSERT_EQ("Unable to retreive the assigned id for the new record.", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertResults_ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true, ""),
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertResults(assessmentSample.getId(), assessmentSample.getResults()));
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertResults_ErrorAtInsert_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(false, "An error occurred while inserting")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.insertResults(assessmentSample.getId(), assessmentSample.getResults()));
    ASSERT_EQ("An error occurred while inserting", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ErrorAtLoadOldResults_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while loading old results")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(assessmentSample));
    ASSERT_EQ("An error occurred while loading old results", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ErrorAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true),
        FakeOperationResultFactory::createNewUpdateResult(false, "An error occurred while updating")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(assessmentSample));
    ASSERT_EQ("An error occurred while updating", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdateWithUpdatingTwoResults_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
        //Updating the two results
        FakeOperationResultFactory::createNewUpdateResult(true),
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdateButErrorAtUpdatingTwoResults_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
        //Updating the two results
        FakeOperationResultFactory::createNewUpdateResult(false, "An error occurred while updating the results")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(assessmentSample));
    ASSERT_EQ("An error occurred while updating the results", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdateWithUpdatingOneResultAndDeletingOneResult_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
        FakeOperationResultFactory::createNewUpdateResult(true), //Updating one result
        FakeOperationResultFactory::createNewDeleteResult(true)  //Deleting one result
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    assessmentSample.removeResult(result1);
    ASSERT_TRUE(storage.updateItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdateWithUpdatingOneResultAndFailAtDeletingOneResult_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
        FakeOperationResultFactory::createNewUpdateResult(true), //Updating one result
        FakeOperationResultFactory::createNewDeleteResult(false, "An error occurred while deleting the results") //Deleting one result
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    assessmentSample.removeResult(result1);
    ASSERT_FALSE(storage.updateItem(assessmentSample));
    ASSERT_EQ("An error occurred while deleting the results", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdateWithDeletingTwoResultsAndInsertingOneResult_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        FakeOperationResultFactory::createNewUpdateResult(true), //Update assessment
        //Deleting the two results
        FakeOperationResultFactory::createNewDeleteResult(true),
        FakeOperationResultFactory::createNewDeleteResult(true),
        FakeOperationResultFactory::createNewInsertResult(true)

    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    assessmentSample.removeResult(result1);
    assessmentSample.removeResult(result2);
    assessmentSample.addResult(AssessmentResult(Student(3, "Santa", "Claus"), 77.7f, ""));
    ASSERT_TRUE(storage.updateItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_ValidWithDeletingZeroResult_ReturnSuccess)
{
auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", {}),
        //Delete assessment
        FakeOperationResultFactory::createNewDeleteResult(true) 

    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(QueryResult::OK, storage.deleteItem(assessmentSample.getId()));
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_ValidWithDeletingTwoResults_ReturnSuccess)
{
auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        //Delete the two assessment results
        FakeOperationResultFactory::createNewDeleteResult(true),
        //Delete assessment
        FakeOperationResultFactory::createNewDeleteResult(true) 

    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(QueryResult::OK, storage.deleteItem(assessmentSample.getId()));
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_FailedAtLoadingTheAssessment_ReturnError)
{
auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "Failed at loading the assessment")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(assessmentSample.getId()));
    ASSERT_EQ("No assessment was returned.", storage.getLastError());

}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_FailedAtRemovingResults_ReturnError)
{
auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        //Delete the two results
        FakeOperationResultFactory::createNewDeleteResult(false, "Failed at deleting the assessment results")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(assessmentSample.getId()));
    ASSERT_EQ("Failed at deleting the assessment results", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_FailedAtRemovingAssessment_ReturnError)
{
auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentSample1 }),
        FakeOperationResultFactory::createNewSelectResult(true, "", { assessmentResultSample1, assessmentResultSample2 }),
        //Delete the two results
        FakeOperationResultFactory::createNewDeleteResult(true),
        //Delete the assessment
        FakeOperationResultFactory::createNewDeleteResult(false, "Failed at deleting the assessment", QueryResult::ERROR)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(assessmentSample.getId()));
    ASSERT_EQ("Failed at deleting the assessment", storage.getLastError());
}