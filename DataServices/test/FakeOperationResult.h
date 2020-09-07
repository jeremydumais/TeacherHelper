#pragma once

#include "queryResult.h"

class FakeOperationResult {
public:
    friend class FakeOperationResultFactory;
    friend class FakeOperationFactory;
private:
    FakeOperationResult(bool executeResult,
                        QueryResult extendedResultInfo,
                        const std::string &lastError,
                        size_t nbOfReturnedItems) 
        : executeResult(executeResult),
          extendedResultInfo(extendedResultInfo),
          lastError(lastError),
          nbOfReturnedItems(nbOfReturnedItems) {}
    bool executeResult;
    QueryResult extendedResultInfo;
    std::string lastError;
    size_t nbOfReturnedItems;
};