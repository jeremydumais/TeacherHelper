#pragma once

#include "IStorageOperationFactory.h"
#include "FakeDeleteOperation.h"
#include "FakeInsertOperation.h"
#include "FakeOperationResult.h"
#include "FakeSelectOperation.h"
#include "FakeUpdateOperation.h"
#include <vector>

class FakeOperationFactory : public IStorageOperationFactory
{
public:
    FakeOperationFactory() = default;

    FakeOperationFactory(const std::vector<FakeOperationResult> &results)
        : operationResults(results)
    {
    }

    std::unique_ptr<IStorageSelectOperation> createSelectOperation(const DatabaseConnection &connection, 
        const std::string &query,
        const std::vector<std::string> &args) override
    {

        const auto &result = operationResults[operationIndex];
        operationIndex++;
        return std::make_unique<FakeSelectOperation>(connection, query, args, result.executeResult, result.lastError, result.nbOfReturnedItems);
    }

    std::unique_ptr<IStorageInsertOperation> createInsertOperation(const DatabaseConnection &connection, 
        const std::string &query,
        const std::vector<std::string> &args) override
    {
        const auto &result = operationResults[operationIndex];
        operationIndex++;
        return std::make_unique<FakeInsertOperation>(connection, query, args, result.executeResult, result.lastError);
    }

    std::unique_ptr<IStorageUpdateOperation> createUpdateOperation(const DatabaseConnection &connection, 
        const std::string &query,
        const std::vector<std::string> &args) override
    {
        const auto &result = operationResults[operationIndex];
        operationIndex++;
        return std::make_unique<FakeUpdateOperation>(connection, query, args, result.executeResult, result.lastError);
    }

    std::unique_ptr<IStorageDeleteOperation> createDeleteOperation(const DatabaseConnection &connection, 
        const std::string &query,
        const std::vector<std::string> &args) override
    {
        const auto &result = operationResults[operationIndex];
        operationIndex++;
        return std::make_unique<FakeDeleteOperation>(connection, query, args, result.executeResult, result.extendedResultInfo, result.lastError);
    }

    std::vector<FakeOperationResult> operationResults;
    size_t operationIndex = 0;
};