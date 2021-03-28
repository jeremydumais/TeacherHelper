#pragma once

#include "IStorageDDLOperation.h"

class FakeDDLOperation : public IStorageDDLOperation
{
public:
    FakeDDLOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          const std::string &lastError)
        : IStorageDDLOperation(connection, query, args),
          executeResult(executeResult) 
          { 
              this->lastError = lastError;
          }
    bool execute() override { return executeResult; }
    bool executeResult = true;
};
