#pragma once

#include "IDatabaseConnectionFactory.h"

class DatabaseConnectionFactory : public IDatabaseConnectionFactory
{
public:
    std::shared_ptr<IDatabaseConnection> createNewConnection(const std::string &databaseName) override;
};