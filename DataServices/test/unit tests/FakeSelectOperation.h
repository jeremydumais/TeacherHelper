#pragma once

#include "IStorageSelectOperation.h"
#include "sqliteDateTimeFactory.h"
#include <boost/any.hpp>
#include <sstream>
#include <string>
#include <vector>

class FakeSelectOperation : public IStorageSelectOperation
{
public:
    FakeSelectOperation(const IDatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args,
                          bool executeResult,
                          const std::string &lastError,
                          const std::vector<std::vector<boost::any>> &returnedItems)
        : IStorageSelectOperation(connection, query, args),
          executeResult(executeResult),
          returnedItems(returnedItems)
          { 
              this->lastError = lastError;
          }

    bool execute() override { 
        currentItemIndex = -1;
        return executeResult; 
    }

    void close() override {}

    bool getRow() override
    {
        bool isARow = (currentItemIndex+1 < returnedItems.size());
        currentItemIndex++;
        return isARow;
    }

    int getIntValue(int columnNumber) const override
    {
        return boost::any_cast<int>(returnedItems[currentItemIndex][columnNumber]);
    }

    std::string getStringValue(int columnNumber) const override
    {
        return boost::any_cast<std::string>(returnedItems[currentItemIndex][columnNumber]);
    }

    bool getBoolValue(int columnNumber) const override
    {
        return boost::any_cast<bool>(returnedItems[currentItemIndex][columnNumber]);
    }

    SQLiteDateTime getDateTime(int columnNumber) const override
    {
        return SQLiteDateTimeFactory::NewDateTimeFromISOExtended(boost::any_cast<std::string>(returnedItems[currentItemIndex][columnNumber]));
    }

    double getDoubleValue(int columnNumber) const override
    {
        return boost::any_cast<double>(returnedItems[currentItemIndex][columnNumber]);
    }

    float getFloatValue(int columnNumber) const override
    {
        return boost::any_cast<float>(returnedItems[currentItemIndex][columnNumber]);
    }

    bool executeResult = true;
    int currentItemIndex = -1;
    std::vector<std::vector<boost::any>> returnedItems;
};
