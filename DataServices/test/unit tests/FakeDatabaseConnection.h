#pragma once

#include "IDatabaseConnection.h"
#include <stdexcept>

class FakeDatabaseConnection : public IDatabaseConnection
{
public:
    const std::string &getDbName() const override { return dbname; };
    void *getConnectionPtr() const override { return nullptr; };
    void open() override {};
    void openForCreation() override { 
        if (isOpenForCreationThrowRuntimeError) {
            throw std::runtime_error("runtimeErrorThrown");
        }
    };
    bool isOpened() const override { return true; };
    void close() override {};
    std::string dbname {""};
    bool isOpenForCreationThrowRuntimeError;
};