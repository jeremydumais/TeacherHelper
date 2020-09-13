#pragma once

#include "IStorageSelectOperation.h"
#include "sqliteDateTimeFactory.h"
#include <sstream>

class FakeSelectOperation : public IStorageSelectOperation
{
public:
    FakeSelectOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          const std::string &lastError,
                          size_t nbOfItems)
        : IStorageSelectOperation(connection, query, args),
          executeResult(executeResult),
          nbOfReturnItems(nbOfItems)
          { 
              this->lastError = lastError;
          }

    bool execute() override { 
        currentItemIndex = 0;
        return executeResult; 
    }

    void close() override {}

    bool getRow() override
    {
        bool isARow = (currentItemIndex < nbOfReturnItems);
        currentItemIndex++;
        return isARow;
    }

    int getIntValue(int columnNumber) const override
    {
        return 1 + currentItemIndex;
    }

    std::string getStringValue(int columnNumber) const override
    {
        std::stringstream ss;
        ss << "SimpleString" << std::to_string(currentItemIndex);
        return ss.str();
    }

    bool getBoolValue(int columnNumber) const override
    {
        return false;
    }

    SQLiteDateTime getDateTime(int columnNumber) const 
    {
        return SQLiteDateTimeFactory::NewDateTimeFromISOExtended("2020-08-08T10:11:12");

    }

    double getDoubleValue(int columnNumber) const
    {
        return 80.2;
    }

    bool executeResult = true;
    size_t nbOfReturnItems = 0;
    size_t currentItemIndex = 0;
};
