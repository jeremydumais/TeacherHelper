#pragma once

#include "FakeOperationResult.h"

class FakeOperationResultFactory {
public:
    FakeOperationResultFactory() = delete;
    static FakeOperationResult createNewSelectResult(bool executeResult,
                                                     const std::string &lastError = "",
                                                     size_t nbOfReturnedItems = 0)
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError, nbOfReturnedItems);
    }

    static FakeOperationResult createNewInsertResult(bool executeResult,
                                                     const std::string &lastError = "")
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError, 0);
    }

    static FakeOperationResult createNewUpdateResult(bool executeResult,
                                                     const std::string &lastError = "")
    {
        return FakeOperationResult(executeResult, QueryResult::OK, lastError, 0);
    }

    static FakeOperationResult createNewDeleteResult(bool executeResult,
                                                     const std::string &lastError = "",
                                                     QueryResult extendedError = QueryResult::OK)
    {
        return FakeOperationResult(executeResult, extendedError, lastError, 0);
    }
};