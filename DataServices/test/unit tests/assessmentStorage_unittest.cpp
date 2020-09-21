#include "assessmentStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <gtest/gtest.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

class AssessmentStorageWithSampleTwoResult : public ::testing::Test
{
public:
	AssessmentStorageWithSampleTwoResult()
	  : assessmentSample(1, "Intra Exam", 
			  TestType("Exam"),
			  Subject("History"),
			  Class("MyClass", School("Test", City("CityTest"))),
				  ptime(date(2020, Aug, 23), time_duration(13, 21, 33))),
		result1 { Student(1, "Joe", "Blow"), 90.5f },
		result2 { Student(2, "Jane", "Doe"), 87.2f }
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
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(true, "", 0)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(2, storage.getAllItems().size());
}

TEST(AssessmentStorage_getAllItems, TwoAssessmentsWithOneResultsNoError_ReturnListAssessments)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    auto items { storage.getAllItems() };
    ASSERT_EQ(2, items.size());
    ASSERT_EQ(1, items.begin()->getResults().size());
}

TEST(AssessmentStorage_getAllItems, ErrorAtFirstSelect_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while loading the assessments.", 0),
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while loading the assessments.", storage.getLastError());
}

TEST(AssessmentStorage_getAllItems, ErrorAtSecondSelect_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while loading the results.", 0),
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));
    ASSERT_EQ(0, storage.getAllItems().size());
    ASSERT_EQ("An error occurred while loading the results.", storage.getLastError());
}

TEST(AssessmentStorage_getItemsByClassId, NoAssessmentForClass_ReturnEmptyList)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewSelectResult(true, "", 2),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    auto items { storage.getItemsByClassId(1) };
    ASSERT_EQ(2, items.size());
    ASSERT_EQ(1, items.begin()->getResults().size());
}

TEST_F(AssessmentStorageWithSampleTwoResult, insertItem_ValidInsert_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
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
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
    ASSERT_EQ(2, storage.retreiveAssignedAssessmentId());
}

TEST_F(AssessmentStorageWithSampleTwoResult, retreiveAssignedAssessmentId_ErrorAtSelect_Return0)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(true, "", 1),
        FakeOperationResultFactory::createNewInsertResult(true, ""),
        FakeOperationResultFactory::createNewSelectResult(false, "An error occurred while selecting", 0)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.insertItem(assessmentSample));
    ASSERT_EQ(0, storage.retreiveAssignedAssessmentId());
    ASSERT_EQ("An error occurred while selecting", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(true)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_TRUE(storage.updateItem(assessmentSample));
}

TEST_F(AssessmentStorageWithSampleTwoResult, updateItem_ErrorAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewUpdateResult(false, "An error occurred while updating")
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_FALSE(storage.updateItem(assessmentSample));
    ASSERT_EQ("An error occurred while updating", storage.getLastError());
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_ValidUpdate_ReturnTrue)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(true)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::OK, storage.deleteItem(assessmentSample.getId()));
}

TEST_F(AssessmentStorageWithSampleTwoResult, deleteItem_ErrorAtUpdate_ReturnFalse)
{
    auto factory { make_unique<FakeOperationFactory>( vector<FakeOperationResult> { 
        FakeOperationResultFactory::createNewDeleteResult(false, "An error occurred while deleting", QueryResult::ERROR)
    }) };
    AssessmentStorage storage(DatabaseConnection("fake"), move(factory));

    ASSERT_EQ(QueryResult::ERROR, storage.deleteItem(assessmentSample.getId()));
    ASSERT_EQ("An error occurred while deleting", storage.getLastError());
}