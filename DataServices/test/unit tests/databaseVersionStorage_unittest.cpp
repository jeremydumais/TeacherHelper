#include "databaseVersionStorage.h"
#include "IStorageOperationFactory.h"
#include "IStorageUpdateOperation.h"
#include "FakeInsertOperation.h"
#include "FakeUpdateOperation.h"
#include "FakeOperationFactory.h"
#include "FakeOperationResultFactory.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>

using namespace std;

/*struct FakeCityRow
{
    int id;
    string name;
    operator vector<boost::any>() const 
    { 
        return vector<boost::any> { id, name }; 
    }
};*/
