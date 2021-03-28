#pragma once

#include "IStorageInsertOperation.h"

class FakeInsertOperation : public IStorageInsertOperation
{
public:
    FakeInsertOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          const std::string &lastError)
        : IStorageInsertOperation(connection, query, args),
          executeResult(executeResult) 
          { 
              this->lastError = lastError;
          }
    bool execute() override { return executeResult; }
    bool executeResult = true;
};
