#pragma once

#include "operationBase.h" 

class IStorageSelectOperation : public OperationBase
{
public:
    IStorageSelectOperation(const DatabaseConnection &connection, 
                          const std::string &query,
                          const std::vector<std::string> &args = std::vector<std::string>())
        : OperationBase(connection, query, args) {}
    virtual ~IStorageSelectOperation() {};
    virtual void close() = 0;
    sqlite3_stmt *getStatement() const;
    virtual bool getRow() = 0;
    virtual int getIntValue(int columnNumber) const = 0;
    virtual std::string getStringValue(int columnNumber) const = 0;
};