#pragma once

#include "FakeDatabaseConnection.h"
#include "IDatabaseConnectionFactory.h"
#include <memory>

class FakeDatabaseConnectionFactory : public IDatabaseConnectionFactory
{
public:
    std::shared_ptr<IDatabaseConnection> createNewConnection(const std::string &databaseName) override
    {

        auto retVal { std::make_shared<FakeDatabaseConnection>() };
        retVal->isOpenForCreationThrowRuntimeError = isOpenForCreationThrowRuntimeError;
        return retVal;
    }
    bool isOpenForCreationThrowRuntimeError;
};