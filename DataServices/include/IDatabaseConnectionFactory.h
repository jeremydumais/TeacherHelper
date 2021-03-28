#pragma once

#include "IDatabaseConnection.h"
#include <memory>
#include <string>

class IDatabaseConnectionFactory
{
public:
    virtual ~IDatabaseConnectionFactory() {};
    virtual std::shared_ptr<IDatabaseConnection> createNewConnection(const std::string &databaseName) = 0;
};