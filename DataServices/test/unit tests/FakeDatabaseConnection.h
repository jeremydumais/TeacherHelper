#pragma once

#include "IDatabaseConnection.h"

class FakeDatabaseConnection : public IDatabaseConnection
{
    const std::string &getDbName() const override { return dbname; };
    void *getConnectionPtr() const { return nullptr; };
    void open() override {};
    bool isOpened() const override { return true; };
    void close() override {};
    std::string dbname {""};
};