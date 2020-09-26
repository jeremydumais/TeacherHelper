#pragma once

#include "queryResult.h"
#include <boost/any.hpp>
#include <vector>

class FakeOperationResult {
public:
    friend class FakeOperationResultFactory;
    friend class FakeOperationFactory;
private:
    FakeOperationResult(bool executeResult,
                        QueryResult extendedResultInfo,
                        const std::string &lastError,
                        const std::vector<std::vector<boost::any>> &returnedItems = std::vector<std::vector<boost::any>>()) 
        : executeResult(executeResult),
          extendedResultInfo(extendedResultInfo),
          lastError(lastError),
          returnedItems(returnedItems) {}
    bool executeResult;
    QueryResult extendedResultInfo;
    std::string lastError;
    std::vector<std::vector<boost::any>> returnedItems;
};