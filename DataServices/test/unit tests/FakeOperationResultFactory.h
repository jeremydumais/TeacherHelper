#pragma once

#include "FakeOperationResult.h"
#include <vector>
#include <boost/any.hpp>

class FakeOperationResultFactory {
public:
    FakeOperationResultFactory() = delete;
    static FakeOperationResult createNewSelectResult(bool executeResult,
                                                     const std::string &lastError = "",
                                                     const std::vector<std::vector<boost::any>> &returnedItems = std::vector<std::vector<boost::any>>{})
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError, returnedItems);
    }

    static FakeOperationResult createNewInsertResult(bool executeResult,
                                                     const std::string &lastError = "")
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError);
    }

    static FakeOperationResult createNewUpdateResult(bool executeResult,
                                                     const std::string &lastError = "")
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError);
    }

    static FakeOperationResult createNewDeleteResult(bool executeResult,
                                                     const std::string &lastError = "",
                                                     QueryResult extendedError = QueryResult::OK)
    {
        return FakeOperationResult(executeResult, extendedError, lastError);
    }
};