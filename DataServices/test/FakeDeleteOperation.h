#pragma once

#include "IStorageDeleteOperation.h"

class FakeDeleteOperation : public IStorageDeleteOperation
{
public:
    FakeDeleteOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          QueryResult extendedResultInfo,
                          const std::string &lastError)
        : IStorageDeleteOperation(connection, query, args),
          executeResult(executeResult),
          extendedResultInfo(extendedResultInfo)
          { 
              this->lastError = lastError;
          }
    bool execute() override { return executeResult; }
    QueryResult getExtendedResultInfo() const override { return extendedResultInfo; }
    bool executeResult = true;
    QueryResult extendedResultInfo;
};