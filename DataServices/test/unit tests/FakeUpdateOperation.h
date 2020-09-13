#pragma once

#include "IStorageUpdateOperation.h"

class FakeUpdateOperation : public IStorageUpdateOperation
{
public:
    FakeUpdateOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          const std::string &lastError)
        : IStorageUpdateOperation(connection, query, args),
          executeResult(executeResult) 
          { 
              this->lastError = lastError;
          }
    bool execute() override { return executeResult; }
    bool executeResult = true;
};